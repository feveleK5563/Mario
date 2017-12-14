//-------------------------------------------------------------------
//vCiผ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/Image/Chara00.png");

		return true;
	}
	//-------------------------------------------------------------------
	//\[Xฬ๐๚
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//u๚ปv^XNถฌษP๑พฏsค
	bool  Object::Initialize()
	{
		//X[p[NX๚ป
		__super::Initialize(defGroupName, defName, true);
		//\[XNXถฌor\[XคL
		this->res = Resource::Create();

		//f[^๚ป
		this->controllerName = "P1";
		this->render2D_Priority[1] = 0.5f;
		this->speed.x = 0;
		this->speed.y = 0;
		this->pos.x = 480 / 2;
		this->pos.y = 270 * 2 / 3;

		//^XNฬถฌ

		return  true;
	}
	//-------------------------------------------------------------------
	//uIนv^XNมลษP๑พฏsค
	bool  Object::Finalize()
	{
		//f[^^XN๐๚


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//๘ซpฌ^XNฬถฌ
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[ษsค
	void  Object::UpDate()
	{
		Object::ChangeSpeed();
		pos.x += speed.x;
		pos.y += speed.y;

		//Jฬฎ
		ML::Box2D copyPlayer = ge->camera2D.OffsetCopy(pos);
		if (speed.x > 0 /*&& copyPlayer.x > 100 && copyPlayer.x < 240*/)
		{
			ge->camera2D.x -= speed.x;
			if (ge->camera2D.x > 0) { ge->camera2D.x = 0; }
			if (ge->camera2D.x < -213 * 16 + ge->camera2D.w) { ge->camera2D.x = -213 * 16 + ge->camera2D.w; }
		}
	}
	//-------------------------------------------------------------------
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
		//LN^`ๆ
		ML::Box2D  draw(-32, -16, 64, 32);
		draw.Offset(this->pos);
		ML::Box2D  src(0, 0, 64, 32);
		draw.Offset(ge->camera2D.x, ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//-------------------------------------------------------------------
	//Xs[hฬฯป
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

	//
	//ศบอ๎{IษฯXsvศ\bh
	//
	//-------------------------------------------------------------------
	//^XNถฌ๛
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWษo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYษธsตฝ็Kill
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
	//\[XNXฬถฌ
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