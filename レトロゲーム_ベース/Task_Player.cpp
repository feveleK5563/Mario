//-------------------------------------------------------------------
//�v���C���i��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Map.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/Image/Chara00.png");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		res = Resource::Create();

		//���f�[�^������
		controllerName = "P1";
		render2D_Priority[1] = 0.5f;
		pos.x = 480 / 2;
		pos.y = 270 * 2 / 3;
		moveVec.x = 0;
		moveVec.y = 0;

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�v���C���[�̓���
		Object::ChangeSpeed();

		//�J�����̋���
		Object::ScrollCamera();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�L�����N�^�`��
		ML::Box2D  draw(-32, -16, 64, 32);
		draw.Offset(this->pos);
		ML::Box2D  src(0, 0, 64, 32);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//-------------------------------------------------------------------
	//�X�s�[�h�̕ω�
	void Object::ChangeSpeed()
	{
		//�R���g���[��������͏����󂯎��
		DI::VGamePad in = DI::GPad_GetState(controllerName);

		if (in.LStick.L.on || (!in.LStick.R.on && moveVec.x > 0)) { --moveVec.x; }
		if (in.LStick.R.on || (!in.LStick.L.on && moveVec.x < 0)) { ++moveVec.x; }

		if (in.LStick.U.on || (!in.LStick.D.on && moveVec.y > 0)) { --moveVec.y; }
		if (in.LStick.D.on || (!in.LStick.U.on && moveVec.y < 0)) { ++moveVec.y; }

		//������Ƃ�������
		if (moveVec.x >  3) { moveVec.x =  3; }
		if (moveVec.x < -3) { moveVec.x = -3; }
		if (moveVec.y >  3) { moveVec.y =  3; }
		if (moveVec.y < -3) { moveVec.y = -3; }

		//�����蔻��t���̓���
		pos.x += moveVec.x;
		pos.y += moveVec.y;
	}

	//-------------------------------------------------------------------
	//�J�����̃X�N���[��
	void Object::ScrollCamera()
	{
		if (moveVec.x > 0)
		{
			//��ʏ�ł̃v���C���[�̍��W
			ML::Vec2 copyPlayer = { pos.x - ge->camera2D.x, pos.y - ge->camera2D.y };
			ge->camera2D.x += int(moveVec.x * ((copyPlayer.x / 60.f) / (240 / 60)));

			if (ge->camera2D.x < 0) { ge->camera2D.x = 0; } //���[
			if (ge->camera2D.x > 213 * 16 - ge->camera2D.w) { ge->camera2D.x = 213 * 16 - ge->camera2D.w; } //�E�[
		}
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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