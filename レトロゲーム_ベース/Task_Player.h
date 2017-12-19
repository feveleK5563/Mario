#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤ（仮
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Player
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤ");	//グループ名
	const  string  defName("マリオ");	//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		string		imageName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		string		controllerName;
		DI::VGamePad in;
		enum Animation
		{
			Stay,
			Walk1,
			Walk2,
			Walk3,
			Turn,
			Jump,
			DeathorSquat,
			Hold1,
			Hold2,
		};
		Animation		marioChip;								//マリオのアニメーション名
		const Animation	walkAnimTable[4] = { Walk1, Walk2, Walk3, Walk2 };	//歩行アニメーション
		float			walkAnimTiming;							//歩行アニメーションのタイミング調整

		int				goalFlag;								//ゴールしたか否か
		const Animation holdAnimTable[2] = { Hold1, Hold2 };	//ポール捕まりアニメーション
		int				holdAnimTiming;							//捕まりアニメーションのタイミング調整

		void SetHitBase();
		void ChangeSpeed();
		void ChangeAnim();
		void ScrollCamera();
		void GameOverEvent();
		void GoalEvent(ML::Vec2&);
	};
}