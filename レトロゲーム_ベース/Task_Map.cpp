//-------------------------------------------------------------------
//�}�b�v�Ǘ�
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map.h"

namespace  Map
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		imageName = "MapChipImg";
		DG::Image_Create(imageName, "./data/image/MapChip.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}

	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.9f;
		//�}�b�v�̃[���N���A
		for (int y = 0; y < 14; ++y)
			for (int x = 0; x < 213; ++x)
				mapData.map[y][x] = 0;
		mapData.sizeX = 0;
		mapData.sizeY = 0;
		mapData.hitBase = ML::Box2D(0, 0, 0, 0);

		//�}�b�v�`�b�v���̏�����
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
			

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		DG::Image_Erase(res->imageName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�J���������S�Ƀ}�b�v�O�ɂ���ꍇ�͕`�������߂�
		if (!mapData.hitBase.Hit(ge->camera2D))
			return;

		//�J�����ƃ}�b�v���d�Ȃ��Ă���͈͂����̋�`�����
		RECT cm = {	//�J�����̋�`
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h
		};
		RECT mp = {	//�}�b�v�̋�`
			mapData.hitBase.x,
			mapData.hitBase.y,
			mapData.hitBase.x + mapData.hitBase.w,
			mapData.hitBase.y + mapData.hitBase.h
		};
		//2�̋�`�̏d�Ȃ��Ă���͈͂����̋�`�����߂�
		RECT isr = {
			max(cm.left, mp.left),
			max(cm.top, mp.top),
			min(cm.right, mp.right),
			min(cm.bottom, mp.bottom)
		};

		//���[�v�͈͂�����
		int sx, sy, ex, ey;
		sx = isr.left / 16;
		sy = isr.top / 16;
		ex = (isr.right - 1) / 16;
		ey = (isr.bottom - 1) / 16;

		//��ʓ��͈̔͂����`��
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
	//�}�b�v�t�@�C���ǂݍ���
	bool Object::Load(const string& fpath)
	{
		//�t�@�C�����J��(�ǂݍ���)
		ifstream fin(fpath);
		if (!fin)
			return false;

		//�`�b�v�t�@�C�����̓ǂݍ��݂Ɖ摜�̃��[�h(�����Resource�ōs���Ă���̂ŕs�v)
		string chipFileName;
		fin >> chipFileName;

		fin >> mapData.sizeX >> mapData.sizeY;
		mapData.hitBase = ML::Box2D(0, 0, mapData.sizeX * 16, mapData.sizeY * 16);

		//�z��Ƀf�[�^����荞��
		for (int y = 0; y < mapData.sizeY; ++y)
			for (int x = 0; x < mapData.sizeX; ++x)
				fin >> mapData.map[y][x];

		//�t�@�C�������
		fin.close();
		return true;
	}
	//-------------------------------------------------------------------
	//�����蔻��Ɠ���
	bool Object::CheckHit(const ML::Box2D& hit, const Check check)
	{
		RECT r = { hit.x, hit.y, hit.x + hit.w, hit.y + hit.h };
		//��`���}�b�v�O�ɏo�Ă�����ۂߍ��݂��s��
		RECT m = {
			mapData.hitBase.x,
			mapData.hitBase.y,
			mapData.hitBase.x + mapData.hitBase.w,
			mapData.hitBase.y + mapData.hitBase.h, };
		if (r.left   < m.left)   { r.left   = m.left; }
		if (r.top    < m.top)    { r.top    = m.top; }
		if (r.right  > m.right)  { r.right  = m.right; }
		if (r.bottom > m.bottom) { r.bottom = m.bottom; }

		//���[�v�͈͒���
		int sx, sy, ex, ey;
		sx = r.left / 16;
		sy = r.top / 16;
		ex = (r.right - 1) / 16;
		ey = (r.bottom -1) / 16;
		
		//�͈͓��̏�Q����T��
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