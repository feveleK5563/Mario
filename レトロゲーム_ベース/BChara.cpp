//-------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-------------------------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map.h"

//-------------------------------------------------------------------
//マップチップとの当たり判定処理(画面外に出ない処理を行う場合はtrueを引数に入力)
//引数　：移動量(ML::Vec2), 画面外停止処理の有無(bool)
//戻り値：問題なし(0), ゲームオーバー(1), ゲームクリア(2)
int BChara::CheckMove(ML::Vec2& est, bool noOutScreen)
{
	//マップが存在するか調べてからアクセス
	auto map = ge->GetTask_One_GN<Map::Object>("フィールド", "マップ");
	if (map == nullptr)
		return 0;

	float *checkE, *checkP;
	int flag = 0;
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
		}

		while (*checkE != 0.f)
		{
			float pre = *checkP;
			if		(*checkE >=  1)	{ *checkP += 1.f;		*checkE -= 1.f; }
			else if (*checkE <= -1) { *checkP -= 1.f;		*checkE += 1.f; }
			else					{ *checkP += *checkE;	*checkE =  0.f; }
			ML::Box2D hit = hitBase.OffsetCopy(pos);

			int checkHit = 0, outScreen = 0;
			if ((checkHit = map->CheckHit(hit, map->Nomal)) ||		//マップチップとの当たり判定
				(outScreen = CheckOutScreen(hit, noOutScreen))) {	//スクリーンとの当たり判定

				if (outScreen == 2) {	//画面外下に落下したら
					return 1;			//ゲームオーバー要請
				}

				*checkP = pre;			//移動をキャンセル

				if (checkHit == 2) {	//ポールに当たったら
					return 2;			//ゲームクリア要請
				}

				if (checkHit == 3) {	//ゴール砦の入り口に接触
					return 3;
				}

				break;
			}
		}
	}
	return flag;
}

//-------------------------------------------------------------------
//画面外判定
int BChara::CheckOutScreen(const ML::Box2D& hit, bool noOutScreen)
{
	if (!noOutScreen) //判定を行わない場合は何もなし
		return 0;

	ML::Box2D check = hit.OffsetCopy(-ge->camera2D.x, -ge->camera2D.y);

	if (check.x < 0)
		return 1;	//画面左外に行ったら動作停止

	if (check.y > int(ge->screen2DHeight))
		return 2;	//画面下外に行ったら死亡

	return 0;
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
		return map->CheckHit(foot, map->Foot);
	else
		return false;
}

//-------------------------------------------------------------------
//頭接触判定
bool BChara::CheckHead(bool canBreak)
{
	//頭の当たり判定を生成する
	ML::Box2D head(	hitBase.x,
					hitBase.y - 1,
					hitBase.w,
					1);
	head.Offset(pos);

	if (auto map = ge->GetTask_One_GN<Map::Object>("フィールド", "マップ"))
		return map->CheckHit(head, map->Head, canBreak);
	else
		return false;
}