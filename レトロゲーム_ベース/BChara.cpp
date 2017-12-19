//-------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-------------------------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map.h"

//-------------------------------------------------------------------
//�}�b�v�`�b�v�Ƃ̓����蔻�菈��(��ʊO�ɏo�Ȃ��������s���ꍇ��true�������ɓ���)
//�����@�F�ړ���(ML::Vec2), ��ʊO��~�����̗L��(bool)
//�߂�l�F���Ȃ�(0), �Q�[���I�[�o�[(1), �Q�[���N���A(2)
int BChara::CheckMove(ML::Vec2& est, bool noOutScreen)
{
	//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
	auto map = ge->GetTask_One_GN<Map::Object>("�t�B�[���h", "�}�b�v");
	if (map == nullptr)
		return 0;

	float *checkE, *checkP;
	int flag = 0;
	for (int xy = 0; xy < 2; ++xy)
	{
		switch (xy)
		{
		case 0: //X���ɑ΂���ړ�
			checkE = &est.x;
			checkP = &pos.x;
			break;

		case 1: //Y���ɑ΂���ړ�
			checkE = &est.y;
			checkP = &pos.y;
			break;
		}

		while (*checkE != 0.f)
		{
			float pre = *checkP;
			if		(*checkE >=  1)	{ *checkP += 1.f;		*checkE -= 1.f; }
			else if (*checkE <= -1) { *checkP -= 1.f;		*checkE += 1.f; }
			else					{ *checkP += *checkE;	*checkE =  0.f; }
			ML::Box2D hit = hitBase.OffsetCopy(pos);

			int checkHit = 0, outScreen = 0;
			if ((checkHit = map->CheckHit(hit, map->Nomal)) ||		//�}�b�v�`�b�v�Ƃ̓����蔻��
				(outScreen = CheckOutScreen(hit, noOutScreen))) {	//�X�N���[���Ƃ̓����蔻��

				if (outScreen == 2) {	//��ʊO���ɗ���������
					return 1;			//�Q�[���I�[�o�[�v��
				}

				*checkP = pre;			//�ړ����L�����Z��

				if (checkHit == 2) {	//�|�[���ɓ���������
					return 2;			//�Q�[���N���A�v��
				}

				if (checkHit == 3) {	//�S�[���Ԃ̓�����ɐڐG
					return 3;
				}

				break;
			}
		}
	}
	return flag;
}

//-------------------------------------------------------------------
//��ʊO����
int BChara::CheckOutScreen(const ML::Box2D& hit, bool noOutScreen)
{
	if (!noOutScreen) //������s��Ȃ��ꍇ�͉����Ȃ�
		return 0;

	ML::Box2D check = hit.OffsetCopy(-ge->camera2D.x, -ge->camera2D.y);

	if (check.x < 0)
		return 1;	//��ʍ��O�ɍs�����瓮���~

	if (check.y > int(ge->screen2DHeight))
		return 2;	//��ʉ��O�ɍs�����玀�S

	return 0;
}

//-------------------------------------------------------------------
//�����ڐG����
bool BChara::CheckFoot()
{
	//�����蔻�����ɂ��đ�����`�𐶐�
	ML::Box2D foot(	hitBase.x,
					hitBase.y + hitBase.h,
					hitBase.w,
					1);
	foot.Offset(pos);

	if (auto map = ge->GetTask_One_GN<Map::Object>("�t�B�[���h", "�}�b�v"))
		return map->CheckHit(foot, map->Foot);
	else
		return false;
}

//-------------------------------------------------------------------
//���ڐG����
bool BChara::CheckHead(bool canBreak)
{
	//���̓����蔻��𐶐�����
	ML::Box2D head(	hitBase.x,
					hitBase.y - 1,
					hitBase.w,
					1);
	head.Offset(pos);

	if (auto map = ge->GetTask_One_GN<Map::Object>("�t�B�[���h", "�}�b�v"))
		return map->CheckHit(head, map->Head, canBreak);
	else
		return false;
}