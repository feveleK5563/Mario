//-------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-------------------------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map.h"

//-------------------------------------------------------------------
//�߂荞�܂Ȃ�����
void BChara::CheckMove(ML::Vec2& est)
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
			if (map->CheckHit(hit))
			{
				*checkP = pre; //�ړ����L�����Z��
				break;
			}
		}
	}
}

//-------------------------------------------------------------------
//�����ڐG����
void BChara::CheckFoot()
{

}