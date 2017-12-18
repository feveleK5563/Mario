//-------------------------------------------------------------------
//マップ管理
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map.h"

namespace  Map
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		imageName = "MapChipImg";
		DG::Image_Create(imageName, "./data/image/MapChip.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		this->render2D_Priority[1] = 0.9f;
		//マップのゼロクリア
		for (int y = 0; y < 14; ++y)
			for (int x = 0; x < 213; ++x)
				mapData.map[y][x] = 0;
		mapData.sizeX = 0;
		mapData.sizeY = 0;
		mapData.hitBase = ML::Box2D(0, 0, 0, 0);

		//マップチップ情報の初期化
		fstream fin("./data/StageData/MapChipType.txt");
		if (!fin)
			return false;

		int xMax, yMax;
		fin >> xMax >> yMax;
		for (int y = 0; y < yMax; ++y)
		{
			for (int x = 0; x < xMax; ++x)
			{
				mapData.chip[(y * 33) + x] = ML::Box2D(x * 16, y * 16, 16, 16);
				fin >> mapData.chipType[(y * 33) + x];
			}
		}
		fin.close();
			

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		DG::Image_Erase(res->imageName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//カメラが完全にマップ外にある場合は描画を取りやめる
		if (!mapData.hitBase.Hit(ge->camera2D))
			return;

		//カメラとマップが重なっている範囲だけの矩形を作る
		RECT cm = {	//カメラの矩形
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h
		};
		RECT mp = {	//マップの矩形
			mapData.hitBase.x,
			mapData.hitBase.y,
			mapData.hitBase.x + mapData.hitBase.w,
			mapData.hitBase.y + mapData.hitBase.h
		};
		//2つの矩形の重なっている範囲だけの矩形を求める
		RECT isr = {
			max(cm.left, mp.left),
			max(cm.top, mp.top),
			min(cm.right, mp.right),
			min(cm.bottom, mp.bottom)
		};

		//ループ範囲を決定
		int sx, sy, ex, ey;
		sx = isr.left / 16;
		sy = isr.top / 16;
		ex = (isr.right - 1) / 16;
		ey = (isr.bottom - 1) / 16;

		//画面内の範囲だけ描画
		for (int y = sy; y <= ey; ++y)
		{
			for (int x = sx; x <= ex; ++x)
			{
				if (mapData.map[y][x] == -1)
					continue;

				ML::Box2D draw(x * 16, y * 16, 16, 16);
				draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
				DG::Image_Draw(res->imageName, draw, mapData.chip[mapData.map[y][x]]);
			}
		}
	}
	//-------------------------------------------------------------------
	//マップファイル読み込み
	bool Object::Load(const string& fpath)
	{
		//ファイルを開く(読み込み)
		ifstream fin(fpath);
		if (!fin)
			return false;

		//チップファイル名の読み込みと画像のロード(今回はResourceで行っているので不要)
		string chipFileName;
		fin >> chipFileName;

		fin >> mapData.sizeX >> mapData.sizeY;
		mapData.hitBase = ML::Box2D(0, 0, mapData.sizeX * 16, mapData.sizeY * 16);

		//配列にデータを取り込む
		for (int y = 0; y < mapData.sizeY; ++y)
			for (int x = 0; x < mapData.sizeX; ++x)
				fin >> mapData.map[y][x];

		//ファイルを閉じる
		fin.close();
		return true;
	}
	//-------------------------------------------------------------------
	//当たり判定と動作
	bool Object::CheckHit(const ML::Box2D& hit, const Check check)
	{
		RECT r = { hit.x, hit.y, hit.x + hit.w, hit.y + hit.h };
		//矩形がマップ外に出ていたら丸め込みを行う
		RECT m = {
			mapData.hitBase.x,
			mapData.hitBase.y,
			mapData.hitBase.x + mapData.hitBase.w,
			mapData.hitBase.y + mapData.hitBase.h, };
		if (r.left   < m.left)   { r.left   = m.left; }
		if (r.top    < m.top)    { r.top    = m.top; }
		if (r.right  > m.right)  { r.right  = m.right; }
		if (r.bottom > m.bottom) { r.bottom = m.bottom; }

		//ループ範囲調整
		int sx, sy, ex, ey;
		sx = r.left / 16;
		sy = r.top / 16;
		ex = (r.right - 1) / 16;
		ey = (r.bottom -1) / 16;
		
		//範囲内の障害物を探す
		for (int y = sy; y <= ey; ++y)
		{
			for (int x = sx; x <= ex; ++x)
			{
				if (mapData.map[y][x] == -1)
					continue;

				switch (mapData.chipType[mapData.map[y][x]])
				{
				case 0:
					return true;

				case 1:
					if (check == Head)
						mapData.map[y][x] = -1;
					return true;

				default:
					continue;
				}
			}
		}
		return false;
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