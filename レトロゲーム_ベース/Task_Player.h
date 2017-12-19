#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���i��
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Player
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�v���C��");	//�O���[�v��
	const  string  defName("�}���I");	//�^�X�N��
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
		//���L����ϐ��͂����ɒǉ�����
		string		imageName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
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
		Animation		marioChip;								//�}���I�̃A�j���[�V������
		const Animation	walkAnimTable[4] = { Walk1, Walk2, Walk3, Walk2 };	//���s�A�j���[�V����
		float			walkAnimTiming;							//���s�A�j���[�V�����̃^�C�~���O����

		int				goalFlag;								//�S�[���������ۂ�
		const Animation holdAnimTable[2] = { Hold1, Hold2 };	//�|�[���߂܂�A�j���[�V����
		int				holdAnimTiming;							//�߂܂�A�j���[�V�����̃^�C�~���O����

		void SetHitBase();
		void ChangeSpeed();
		void ChangeAnim();
		void ScrollCamera();
		void GameOverEvent();
		void GoalEvent(ML::Vec2&);
	};
}