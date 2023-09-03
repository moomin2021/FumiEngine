#include "Boss0.h"
#include "CollisionAttribute.h"

Boss0::Boss0()
{
}

Boss0::~Boss0()
{
	colMgr_->RemoveCollider(collider_.get());
}

void Boss0::Initialize(const Vector3& pos)
{
#pragma region インスタンス
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region モデル
	model_ = std::make_unique<Model>("boss0");
#pragma endregion

#pragma region オブジェクト
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition(pos);
	object_->SetScale({ 5.0f, 5.0f, 5.0f });
#pragma endregion

#pragma region コライダー
	collider_ = std::make_unique<SphereCollider>(Vector3{ 0.0f, 7.5f * 5.0, 0.0f }, 5.0f);
	collider_->SetAttribute(COL_ENEMY);
	collider_->SetObject3D(object_.get());
	colMgr_->AddCollider(collider_.get());
#pragma endregion
}

void Boss0::Update()
{
	if (damageCounter_ >= 3) {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
	else {
		damageCounter_++;
	}
}

void Boss0::Draw()
{
	object_->Draw();
}

void Boss0::ObjUpdate()
{
	object_->Update();
}

void Boss0::OnCollision()
{
	if (collider_->GetIsHit()) {
		hp_ -= 1;
		object_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		damageCounter_ = 0;
	}
}