#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�L�����N�^�ėp�N���X
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������������������
	//�L�����N�^���ʃ����o�ϐ�

	enum State	//�ėp��ԊǗ�
	{
		Non,		//�`����X�V�����Ȃ�
		State1,		//���1
		State2,		//���2
		State3,		//���3
	};
	State state;

	ML::Vec2	pos;		//�L�����N�^�ʒu
	ML::Box2D	hitBase;	//�����蔻��͈�
	ML::Vec2	moveVec;	//�ړ��x�N�g��
	int			cntTime;	//�ėp�^�C�}�[

	//�������x
	float	fallSpeed;

	//�ڐG����
	bool	hitFoot; //�����ڐG����
	bool	hitHead; //���ڐG����

	//�A�j���[�V����
	vector<ML::Box2D*>	charaChip;	//�L�����N�^�̑f��
	float				animCnt;	//�A�j���[�V�����J�E���^

	//���E�̌���(2D�����_�Q�[����p)
	enum AngleLR{Left, Right};
	AngleLR angleLR;

	//�����o�ϐ��̏�����
	BChara():
		state(Non),
		pos(0.f, 0.f),
		hitBase(0, 0, 0, 0),
		moveVec(0.f, 0.f),
		cntTime(0),
		fallSpeed(0),
		hitFoot(0),
		hitHead(0),
		angleLR(Right),
		animCnt(0.f){}

	virtual ~BChara(){}

	//--------------------------------------------------------
	//�ȉ��L�����N�^���ʃ��\�b�h

	//�߂荞�܂Ȃ�����
	virtual int CheckMove(ML::Vec2&, bool = false);
	//��ʊO����
	virtual int CheckOutScreen(const ML::Box2D&, bool);
	//�����ڐG����
	virtual bool CheckFoot();
	//���ڐG����
	virtual bool CheckHead(bool);
};
