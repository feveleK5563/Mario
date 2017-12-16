//-------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-------------------------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map.h"

//-------------------------------------------------------------------
//めり込まない処理
void BChara::CheckMove(ML::Vec2& est)
{
	//マップが存在するか調べてからアクセス
	auto map = ge->GetTask_One_GN<Map::Object>("フィールド", "マップ");
	if (map == nullptr)
		return;

	float *checkE, *checkP;

	for (int xy = 0; xy < 2; ++xy)
	{
		switch (xy)
		{
		case 0: //X軸に対する移動
			checkE = &est.x;
			checkP = &pos.x;
			break;

		case 1: //Y軸に対する移動
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
				*checkP = pre; //移動をキャンセル
				break;
			}
		}
	}
}

//-------------------------------------------------------------------
//足元接触判定
void BChara::CheckFoot()
{

}