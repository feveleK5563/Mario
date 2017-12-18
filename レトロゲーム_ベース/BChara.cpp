//-------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-------------------------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map.h"

//-------------------------------------------------------------------
//めり込まない処理(画面外に出ない処理を行う場合はtrueを引数に入力)
void BChara::CheckMove(ML::Vec2& est, bool noOutScreen)
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
			if (map->CheckHit(hit) || CheckOutScreen(hit, noOutScreen))
			{
				*checkP = pre; //移動をキャンセル
				break;
			}
		}
	}
}

//-------------------------------------------------------------------
//画面外判定
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
//足元接触判定
bool BChara::CheckFoot()
{
	//当たり判定を基にして足元矩形を生成
	ML::Box2D foot(	hitBase.x,
					hitBase.y + hitBase.h,
					hitBase.w,
					1);
	foot.Offset(pos);

	if (auto map = ge->GetTask_One_GN<Map::Object>("フィールド", "マップ"))
		return map->CheckHit(foot);
	else
		return false;
}

//-------------------------------------------------------------------
//頭接触判定
bool BChara::CheckHead()
{
	//頭の当たり判定を生成する
	ML::Box2D head(	hitBase.x,
					hitBase.y - 1,
					hitBase.w,
					1);
	head.Offset(pos);

	if (auto map = ge->GetTask_One_GN<Map::Object>("フィールド", "マップ"))
		return map->CheckHit(head);
	else
		return false;
}