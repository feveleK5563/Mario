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
		DG::Image_Create(this->imageName, "./data/Image/chara.png");

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
		pos.x = 480 / 5;
		pos.y = 270 * 2 / 3;
		hitBase = { -6, -8, 12, 16 };
		moveVec.x = 0;
		moveVec.y = 0;
		fallSpeed = 0;
		hitFoot = false;
		angleLR = Right;
		animCnt = 0.f;

		marioChip = Stay;
		transMario = 1;
		walkAnimTiming = 0.f;

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

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		int size = charaChip.size();
		for (int i = 0; i < size; ++i)
			delete charaChip[i];
		charaChip.clear();
		charaChip.shrink_to_fit();

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
		ML::Box2D  draw = { -8, -8, 16, 16 };
		draw.Offset(pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D src = *charaChip[marioChip + (transMario * 9)];
		if (angleLR == Left)
		{
			src.x += 16;
			src.w *= -1;
		}
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//-------------------------------------------------------------------
	//�X�s�[�h�̕ω�
	void Object::ChangeSpeed()
	{
		//�R���g���[��������͏����󂯎��
		DI::VGamePad in = DI::GPad_GetState(controllerName);

		ML::Vec2 est = { 0, 0 };
		//���炩�ɉ��ړ�
		if (in.LStick.L.on || (!in.LStick.R.on && moveVec.x > 0)) {
			est.x += -0.2f + moveVec.x;
		}
		if (in.LStick.R.on || (!in.LStick.L.on && moveVec.x < 0)) {
			est.x +=  0.2f + moveVec.x;
		}
		if (hitFoot) { //�����̕ύX
			if (moveVec.x > 0.f) { angleLR = Right; }
			if (moveVec.x < 0.f) { angleLR = Left; }
		}

		//x���X�s�[�h��0.15�����ɂȂ�����0�ɐݒ肷��
		if (fabsf(est.x) < 0.1f) {
			est.x = 0.f;
		}
		if (in.B2.on)
		{
			//x���X�s�[�h��1.7�ȏ�ɂȂ�Ȃ��悤�ɂ���
			if (est.x >  3.f) { est.x =  3.f; }
			if (est.x < -3.f) { est.x = -3.f; }
		}
		else
		{
			//x���X�s�[�h��1.7�ȏ�ɂȂ�Ȃ��悤�ɂ���
			if (est.x >  1.7f) { est.x =  1.7f; }
			if (est.x < -1.7f) { est.x = -1.7f; }
		}

		//�W�����v����
		if (in.B1.down && hitFoot) {
			fallSpeed = -5.5f;
		}
		if (in.B1.up && fallSpeed < -1.5f) {
			fallSpeed = -1.5f;
		}
		est.y += fallSpeed;

		//����O�̍��W��ۑ�
		ML::Vec2 beforePos = pos;
		//�����蔻��t���̓���
		CheckMove(est, true);

		//�����ڐG����
		if (hitFoot = CheckFoot()) {
			fallSpeed = 0.f;
		}
		else
			fallSpeed += 0.2f;

		//���ڐG����
		if (CheckHead())
			fallSpeed += 1.f;

		//����O�̍��W�Ƃ̍�����A�ǂ̒��x�}���I�������������L�^
		moveVec = ML::Vec2(pos.x - beforePos.x, pos.y - beforePos.y);

		//�}���I�̃A�j���[�V�������s��
		ChangeAnim(in);
	}

	//-------------------------------------------------------------------
	//�}���I�̃A�j���[�V����
	void Object::ChangeAnim(const DI::VGamePad& in)
	{
		//�W�����v
		if (in.B1.down) {
			marioChip = Jump;
			return;
		}

		//�ȉ��A�n�ʂɂ��Ă���Ƃ��̏���
		if (!hitFoot)
			return;

		//���E�ړ�
		if (in.LStick.L.on || in.LStick.R.on) {
			walkAnimTiming += fabsf(moveVec.x);
			marioChip = walkAnimTable[int(walkAnimTiming / 10.f) % 4];
		}
		
		//���씽�]
		if ((in.LStick.L.on && moveVec.x > 0.f) ||
			(in.LStick.R.on && moveVec.x < 0.f))
		{
			marioChip = Turn;
			walkAnimTiming = 0.f;
		}

		//�ҋ@
		if (moveVec.x == 0.f) {
			marioChip = Stay;
			walkAnimTiming = 0.f;
		}
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

			copyPlayer = { pos.x - ge->camera2D.x, pos.y - ge->camera2D.y };
			while (copyPlayer.x > 240.f)
			{
				ge->camera2D.x += 1;
				copyPlayer = { pos.x - ge->camera2D.x, pos.y - ge->camera2D.y };
			}

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