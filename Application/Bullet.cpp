#include "Bullet.h"

// 弾のモデル
Model* Bullet::mBullet_ = nullptr;

Bullet::Bullet(const float3& iniPos, const Vector3& moveVec) :
#pragma region 初期化リスト
	// 弾のオブジェクト
	oBullet_(nullptr),

	// 衝突判定用
	//col_{},

	// 弾の移動方向
	moveVec_(moveVec),

	// 弾の速度
	bulletSpd_(20.0f),

	// 生存フラグ
	isAlive_(true),

	// 生存時間
	aliveCounter_(0),	// 生成からのカウント
	maxCount_(300)		// 最大カウント
#pragma endregion
{
	// オブジェクト生成＆設定
	oBullet_ = std::make_unique<Object3D>(mBullet_);
	oBullet_->SetPosition(iniPos);				// 初期座標の設定
	oBullet_->SetScale({ 0.1f, 0.1f, 0.1f });	// 大きさ設定

	// 移動方向を正規化
	moveVec_.normalize();
}

void Bullet::Update()
{
	// 生存フラグが[OFF]ならこの後の処理を飛ばす
	if (isAlive_ == false) return;

	//col_.pos = oBullet_->GetPosition();
	//col_.radius = oBullet_->GetScale().x;

	// 弾を移動させる
	oBullet_->SetPosition(oBullet_->GetPosition() + moveVec_ * bulletSpd_);
	
	oBullet_->Update();

	// カウントを進める
	aliveCounter_++;

	// 生存カウントが最大カウントを超えたら生存フラグを[OFF]にする
	if (aliveCounter_ >= maxCount_) isAlive_ = false;
}

void Bullet::Draw()
{
	oBullet_->Draw();
}