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
	ML::Vec2	pos;		//�L�����N�^�ʒu
	ML::Box2D	hitBase;	//�����蔻��͈�
	ML::Vec2	moveVec;	//�ړ��x�N�g��
	int			moveCnt;	//�s���J�E���^

	//�A�j���[�V����
	vector<ML::Box2D*>	charaChip;	//�L�����N�^�̑f��
	int					animCnt;	//�A�j���[�V�����J�E���^

	//���E�̌���(2D�����_�Q�[����p)
	enum AngleLR{Left, Right};
	AngleLR angleLR;

	//�����o�ϐ��̏�����
	BChara():
		pos(0.f, 0.f),
		hitBase(0, 0, 0, 0),
		moveVec(0.f, 0.f),
		angleLR(Right),
		animCnt(0) {}

	virtual ~BChara(){}

	//--------------------------------------------------------
	//�ȉ��L�����N�^���ʃ��\�b�h

	//�߂荞�܂Ȃ�����
	virtual void CheckMove(ML::Vec2& est);
	//�����ڐG����
	virtual void CheckFoot();
};
