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
		//�}�b�v�`�b�v���̏�����
		for (int y = 0; y < 28; ++y)
			for (int x = 0; x < 33; ++x)
				mapData.chip[(y * 33) + x] = ML::Box2D(x * 16, y * 16, 16, 16);

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
	//�}�b�v�t�@�C���ǂݍ���
	bool Object::Load(const string& fpath)
	{
		//�t�@�C�����J��(�ǂݍ���)
		ifstream fin(fpath);
		if (!fin)
			return false;

		//�`�b�v�t�@�C�����̓ǂݍ��݂Ɖ摜�̃��[�h
		string chipFileName, chipFilePath;
		fin >> chipFileName;
		chipFilePath = "./data/image/" + chipFileName;
		DG::Image_Create(res->imageName, chipFilePath);

		fin >> mapData.sizeX >> mapData.sizeY;
		mapData.hitbase = ML::Box2D(0, 0, mapData.sizeX * 16, mapData.sizeY * 16);

		//�z��Ƀf�[�^����荞��
		for (int y = 0; y < mapData.sizeY; ++y)
			for (int x = 0; x < mapData.sizeX; ++x)
				fin >> mapData.map[y][x];

		//�t�@�C�������
		fin.close();
		return true;
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