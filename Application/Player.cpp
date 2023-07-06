#include "Player.h"
#include "SphereCollider.h"

void Player::Initialize()
{
	// インスタンス取得
	key_ = Key::GetInstance();

	// コライダーの追加
	SetCollider(new SphereCollider({ 0.0f, 0.0f, 0.0f }, 1.0f));
}

void Player::Update()
{
	static float3 position = {0.0f, 0.0f, 0.0f};

	// [A]、[D]で旋回
	if (key_->PushKey(DIK_A)) {
		position.x -= 0.1f;
	}
	if (key_->PushKey(DIK_D)) {
		position.x += 0.1f;
	}

	SetPosition(position);
	SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
}

void Player::OnCollision(const CollisionInfo& info)
{
	info.object->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
}