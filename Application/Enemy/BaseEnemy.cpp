#include "BaseEnemy.h"
#include "CollisionAttribute.h"

BaseEnemy::BaseEnemy(CollisionManager3D* inColMgr) : colMgr_(inColMgr) {}

void BaseEnemy::HeadHit()
{
	// 当たっていないなら処理を飛ばす
	if (headC_->GetIsHit() == false) return;

	// 当たったのがプレイヤーの弾以外なら処理を飛ばす
	if (headC_->GetHitCollider()->GetAttribute() != COL_PLAYER_SHOT) return;

	// HPを更新
	nowHP_ = 0;
}

void BaseEnemy::BodyHit()
{
	// 当たっていないなら処理を飛ばす
	if (bodyC_->GetIsHit() == false) return;

	// 当たったのがプレイヤーの弾以外なら処理を飛ばす
	if (bodyC_->GetHitCollider()->GetAttribute() != COL_PLAYER_SHOT) return;

	// HPを更新
	nowHP_ -= 1;
}

void BaseEnemy::JudDeath()
{
	// HPが0以下なら生存フラグを[FALSE]にする
	if (nowHP_ <= 0) isAlive_ = false;
}