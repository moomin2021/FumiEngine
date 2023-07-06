#include "Enemy.h"
#include "SphereCollider.h"

void Enemy::Initialize()
{
	// インスタンス取得
	key_ = Key::GetInstance();

	// コライダーの追加
	SetCollider(new SphereCollider({0.0f, 0.0f, 0.0f}, 1.0f));
}

void Enemy::Update()
{
	//static float3 position = { 0.0f, 0.0f, 0.0f };

	//// [A]、[D]で旋回
	//if (key_->PushKey(DIK_A)) {
	//	position.x -= 1.0f;
	//}
	//if (key_->PushKey(DIK_D)) {
	//	position.x += 1.0f;
	//}

	//SetPosition(position);

	SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	//info.object->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
}