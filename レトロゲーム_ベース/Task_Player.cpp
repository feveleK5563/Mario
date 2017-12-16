//-------------------------------------------------------------------
//プレイヤ（仮
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Map.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/Image/chara.png");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		res = Resource::Create();

		//★データ初期化
		controllerName = "P1";
		render2D_Priority[1] = 0.5f;
		pos.x = 480 / 5;
		pos.y = 270 * 2 / 3;
		moveVec.x = 0;
		moveVec.y = 0;
		hitBase = { -6, -8, 12, 16 };

		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 9; ++j)
			{
				if (i == 0)
					charaChip.emplace_back(new ML::Box2D(j * 16 + 80, 0, 16, 32));
				else
					charaChip.emplace_back(new ML::Box2D(j * 16 + 80, 32, 16, 16));
			}
		}
		animCnt = 0;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		int size = charaChip.size();
		for (int i = 0; i < size; ++i)
			delete charaChip[i];
		charaChip.clear();
		charaChip.shrink_to_fit();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//プレイヤーの動作
		Object::ChangeSpeed();

		//カメラの挙動
		Object::ScrollCamera();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//キャラクタ描画
		ML::Box2D  draw = { -8, -8, 16, 16 };
		draw.Offset(pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, *charaChip[9]);
	}
	//-------------------------------------------------------------------
	//スピードの変化
	void Object::ChangeSpeed()
	{
		//コントローラから入力情報を受け取る
		DI::VGamePad in = DI::GPad_GetState(controllerName);

		ML::Vec2 est = { 0, 0 };
		//ちょっとずつ動かす
		if (in.LStick.L.on || (!in.LStick.R.on && moveVec.x > 0)) { est.x += -0.2f + moveVec.x; angleLR = Left; }
		if (in.LStick.R.on || (!in.LStick.L.on && moveVec.x < 0)) { est.x +=  0.2f + moveVec.x; angleLR = Right; }
		if (in.LStick.U.on || (!in.LStick.D.on && moveVec.y > 0)) { est.y += -0.2f + moveVec.y; }
		if (in.LStick.D.on || (!in.LStick.U.on && moveVec.y < 0)) { est.y +=  0.2f + moveVec.y; }

		//0.2未満になったら0に設定する
		if (fabsf(est.x) < 0.2f) { est.x = 0.f; }
		if (fabsf(est.y) < 0.2f) { est.y = 0.f; }
		//3以上にならないようにする
		if (est.x >  3) { est.x =  3; }
		if (est.x < -3) { est.x = -3; }
		if (est.y >  3) { est.y =  3; }
		if (est.y < -3) { est.y = -3; }

		ML::Vec2 beforePos = pos;
		//当たり判定付きの動作
		CheckMove(est);

		moveVec = ML::Vec2(pos.x - beforePos.x, pos.y - beforePos.y);
	}

	//-------------------------------------------------------------------
	//カメラのスクロール
	void Object::ScrollCamera()
	{
		if (moveVec.x > 0)
		{
			//画面上でのプレイヤーの座標
			ML::Vec2 copyPlayer = { pos.x - ge->camera2D.x, pos.y - ge->camera2D.y };
			ge->camera2D.x += int(moveVec.x * ((copyPlayer.x / 60.f) / (240 / 60)));

			copyPlayer = { pos.x - ge->camera2D.x, pos.y - ge->camera2D.y };
			while (copyPlayer.x > 240.f)
			{
				ge->camera2D.x += 1;
				copyPlayer = { pos.x - ge->camera2D.x, pos.y - ge->camera2D.y };
			}

			if (ge->camera2D.x < 0) { ge->camera2D.x = 0; } //左端
			if (ge->camera2D.x > 213 * 16 - ge->camera2D.w) { ge->camera2D.x = 213 * 16 - ge->camera2D.w; } //右端
		}
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}