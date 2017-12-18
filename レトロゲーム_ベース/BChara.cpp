//-------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-------------------------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map.h"

//-------------------------------------------------------------------
//�߂荞�܂Ȃ�����(��ʊO�ɏo�Ȃ��������s���ꍇ��true�������ɓ���)
void BChara::CheckMove(ML::Vec2& est, bool noOutScreen)
{
	//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
	auto map = ge->GetTask_One_GN<Map::Object>("�t�B�[���h", "�}�b�v");
	if (map == nullptr)
		return;

	float *checkE, *checkP;
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

		default:
			return;
		}

		while (*checkE != 0.f)
		{
			float pre = *checkP;
			if		(*checkE >=  1)	{ *checkP += 1.f;		*checkE -= 1.f; }
			else if (*checkE <= -1) { *checkP -= 1.f;		*checkE += 1.f; }
			else					{ *checkP += *checkE;	*checkE =  0.f; }
			ML::Box2D hit = hitBase.OffsetCopy(pos);
			if (map->CheckHit(hit) || CheckOutScreen(hit, noOutScreen))
			{
				*checkP = pre; //�ړ����L�����Z��
				break;
			}
		}
	}
}

//-------------------------------------------------------------------
//��ʊO����
bool BChara::CheckOutScreen(const ML::Box2D& hit, bool noOutScreen)
{
	if (!noOutScreen)
		return false;

	ML::Box2D check = hit.OffsetCopy(-ge->camera2D.x, -ge->camera2D.y);
	if (check.x < 0)
		return true;

	return false;
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
		return map->CheckHit(foot);
	else
		return false;
}

//-------------------------------------------------------------------
//���ڐG����
bool BChara::CheckHead()
{
	//���̓����蔻��𐶐�����
	ML::Box2D head(	hitBase.x,
					hitBase.y - 1,
					hitBase.w,
					1);
	head.Offset(pos);

	if (auto map = ge->GetTask_One_GN<Map::Object>("�t�B�[���h", "�}�b�v"))
		return map->CheckHit(head);
	else
		return false;
}