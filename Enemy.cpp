#include "Enemy.h"

Model* Enemy::model_ = nullptr;

Enemy::Enemy() :
#pragma region メンバ変数
	// オブジェクト
	obj_(nullptr),

	// 衝突判定用
	col_{},

	// HP
	hp_(3),

	// 生存フラグ
	isAlive_(true)
#pragma endregion
{
	obj_ = std::make_unique<Object3D>(model_);
}

void Enemy::Update()
{
	// 生存フラグが[OFF]ならこの後の処理を飛ばす
	if (isAlive_ == false) return;

	col_.pos = obj_->GetPosition();
	col_.radius = obj_->GetScale().x;

	// HPが0以下になったら生存フラグを[OFF]にする
	if (hp_ <= 0) isAlive_ = false;
}

void Enemy::Draw()
{
	obj_->Draw();
}