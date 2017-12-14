//-------------------------------------------------------------------
//プレイヤ（仮
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/Image/Chara00.png");

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
		this->res = Resource::Create();

		//★データ初期化
		this->controllerName = "P1";
		this->render2D_Priority[1] = 0.5f;
		this->speed.x = 0;
		this->speed.y = 0;
		this->pos.x = 480 / 2;
		this->pos.y = 270 * 2 / 3;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		Object::ChangeSpeed();
		pos.x += speed.x;
		pos.y += speed.y;

		//カメラの挙動
		ML::Box2D copyPlayer = ge->camera2D.OffsetCopy(pos);
		if (speed.x > 0 /*&& copyPlayer.x > 100 && copyPlayer.x < 240*/)
		{
			ge->camera2D.x -= speed.x;
			if (ge->camera2D.x > 0) { ge->camera2D.x = 0; }
			if (ge->camera2D.x < -213 * 16 + ge->camera2D.w) { ge->camera2D.x = -213 * 16 + ge->camera2D.w; }
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//キャラクタ描画
		ML::Box2D  draw(-32, -16, 64, 32);
		draw.Offset(this->pos);
		ML::Box2D  src(0, 0, 64, 32);
		draw.Offset(ge->camera2D.x, ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//-------------------------------------------------------------------
	//スピードの変化
	void Object::ChangeSpeed()
	{
		DI::VGamePad in = DI::GPad_GetState(controllerName);

		if (in.LStick.L.on || (!in.LStick.R.on && speed.x > 0)) { --speed.x; }
		if (in.LStick.R.on || (!in.LStick.L.on && speed.x < 0)) { ++speed.x; }

		if (in.LStick.U.on || (!in.LStick.D.on && speed.y > 0)) { --speed.y; }
		if (in.LStick.D.on || (!in.LStick.U.on && speed.y < 0)) { ++speed.y; }

		if (speed.x >  3) { speed.x =  3; }
		if (speed.x < -3) { speed.x = -3; }
		if (speed.y >  3) { speed.y =  3; }
		if (speed.y < -3) { speed.y = -3; }

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