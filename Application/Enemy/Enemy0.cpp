#include "Enemy0.h"
#include "CollisionManager.h"

Enemy0::Enemy0(Model* model)
{
	// オブジェクト生成
	object_ = std::make_unique<Object3D>(model);
}

Enemy0::~Enemy0()
{
	// コライダーを削除
	CollisionManager::GetInstance()->RemoveCollider(collider_.get());
}

void Enemy0::Initialize(float3 pos)
{
	// 初期位置設定
	object_->SetPosition(pos);

	// コライダー生成
	collider_ = std::make_unique<SphereCollider>(pos);

	// コライダーとオブジェクトを紐づけ
	collider_->LinkObject3D(object_.get());

	// コライダーを追加
	CollisionManager::GetInstance()->AddCollider(collider_.get());
}

void Enemy0::Update()
{
	// オブジェクト更新
	object_->Update();
}

void Enemy0::Draw()
{
	// オブジェクト描画
	object_->Draw();
}