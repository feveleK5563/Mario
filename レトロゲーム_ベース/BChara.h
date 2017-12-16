#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//キャラクタ汎用クラス
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ変数
	ML::Vec2	pos;		//キャラクタ位置
	ML::Box2D	hitBase;	//当たり判定範囲
	ML::Vec2	moveVec;	//移動ベクトル
	int			moveCnt;	//行動カウンタ

	//アニメーション
	vector<ML::Box2D*>	charaChip;	//キャラクタの素材
	int					animCnt;	//アニメーションカウンタ

	//左右の向き(2D横視点ゲーム専用)
	enum AngleLR{Left, Right};
	AngleLR angleLR;

	//メンバ変数の初期化
	BChara():
		pos(0.f, 0.f),
		hitBase(0, 0, 0, 0),
		moveVec(0.f, 0.f),
		angleLR(Right),
		animCnt(0) {}

	virtual ~BChara(){}

	//--------------------------------------------------------
	//以下キャラクタ共通メソッド

	//めり込まない処理
	virtual void CheckMove(ML::Vec2& est);
	//足元接触判定
	virtual void CheckFoot();
};
