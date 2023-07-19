#include "Enemy0.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

Enemy0::Enemy0(Model* model) :
	// HP
	hp_(3),

	damageCounter_(0)
{
	// オブジェクト生成
	object_ = std::make_unique<Object3D>(model);
}

Enemy0::~Enemy0()
{
	// コライダーを削除
	CollisionManager::GetInstance()->RemoveCollider(collider_.get());
}

void Enemy0::Initialize(float3 pos, float3 scale)
{
	// 初期位置設定
	object_->SetPosition(pos);
	object_->SetScale(scale);

	// コライダー生成
	collider_ = std::make_unique<SphereCollider>(float3{0.0f, 0.0f, 0.0f}, 2.0f);

	// コライダーとオブジェクトを紐づけ
	collider_->LinkObject3D(object_.get());

	// コライダーに属性を追加
	collider_->SetAttribute(COL_ENEMY);

	// コライダーを追加
	CollisionManager::GetInstance()->AddCollider(collider_.get());
}

void Enemy0::Update()
{
	// 生存フラグが[OFF]ならこの後の処理を飛ばす
	if (isAlive_ == false) return;

	if (damageCounter_ >= 3) {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
	else {
		damageCounter_++;
	}

	// HPが0以下になったら生存フラグを[OFF]にする
	if (hp_ <= 0) isAlive_ = false;

	// 衝突時の処理
	OnCollision();

	// オブジェクト更新
	object_->Update();
}

void Enemy0::Draw()
{
	// オブジェクト描画
	if (isAlive_) object_->Draw();
}

void Enemy0::OnCollision()
{
	// 衝突していなかったら処理を飛ばす
	if (collider_->GetIsHit() == false) return;
	hp_ -= 1;
	object_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
	damageCounter_ = 0;
}