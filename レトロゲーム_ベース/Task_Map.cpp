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
		//マップチップ情報の初期化
		for (int y = 0; y < 28; ++y)
			for (int x = 0; x < 33; ++x)
				mapData.chip[(y * 33) + x] = ML::Box2D(x * 16, y * 16, 16, 16);

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
		for (int y = 0; y < 14; ++y)
		{
			for (int x = 0; x < 213; ++x)
			{
				if (mapData.map[y][x] == -1)
					continue;

				ML::Box2D draw(x * 16, y * 16, 16, 16);
				draw.Offset(ge->camera2D.x, ge->camera2D.y);
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

		//チップファイル名の読み込みと画像のロード
		string chipFileName, chipFilePath;
		fin >> chipFileName;
		chipFilePath = "./data/image/" + chipFileName;
		DG::Image_Create(res->imageName, chipFilePath);

		fin >> mapData.sizeX >> mapData.sizeY;
		mapData.hitbase = ML::Box2D(0, 0, mapData.sizeX * 16, mapData.sizeY * 16);

		//配列にデータを取り込む
		for (int y = 0; y < mapData.sizeY; ++y)
			for (int x = 0; x < mapData.sizeX; ++x)
				fin >> mapData.map[y][x];

		//ファイルを閉じる
		fin.close();
		return true;
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