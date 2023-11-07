#include "Zombie.h"

#include "Util.h"
#include "CollisionAttribute.h"

CollisionManager* Zombie::sColMgr_ = nullptr;
Model* Zombie::sModel_ = nullptr;

Zombie::~Zombie()
{
	sColMgr_->RemoveCollider(cRay_.get());
	sColMgr_->RemoveCollider(cSphere_.get());
}

void Zombie::Initialize(const Vector3& inPos)
{
#pragma region オブジェクト
	object_ = std::make_unique<Object3D>(sModel_);
	object_->SetPosition(inPos);
#pragma endregion

#pragma region コライダー
	cRay_ = std::make_unique<RayCollider>();
	cRay_->SetOffSet({0.0f, 2.0f, 0.0f});
	cRay_->SetDir({ 0.0f, -1.0f, 0.0f });
	cRay_->SetAttribute(COL_LEG);
	cRay_->SetObject3D(object_.get());
	sColMgr_->AddCollider(cRay_.get());

	cSphere_ = std::make_unique<SphereCollider>(Vector3{ 0.0f, 1.0f, 0.0f }, 1.0f);
	cSphere_->SetAttribute(COL_ENEMY);
	cSphere_->SetObject3D(object_.get());
	sColMgr_->AddCollider(cSphere_.get());
#pragma endregion
}

void Zombie::Update()
{
	// 状態別更新処理
	(this->*stateTable[(uint16_t)state_])();

	// 重力処理
	Gravity();
}

void Zombie::Draw()
{
	object_->Draw();
}

void Zombie::OnCollision()
{
	// 接地判定
	GroundingJudgment();
}

void Zombie::MatUpdate()
{
	object_->MatUpdate();
}

void (Zombie::* Zombie::stateTable[]) () = {
	&Zombie::Patrol,	// 見回り状態
	&Zombie::Chase,		// 追跡状態
};

void Zombie::Patrol()
{

}

void Zombie::Chase()
{

}

void Zombie::GroundingJudgment()
{
	if (cRay_->GetIsHit() && cRay_->GetDistance() < 2.0f)
	{
		Vector3 result = object_->GetPosition() + Vector3{0.0f, 2.0f - cRay_->GetDistance(), 0.0f};
		object_->SetPosition(result);
		isGround_ = true;
	}
}

void Zombie::Gravity()
{
	velocity_ += accel_;
	velocity_ = Util::Min(velocity_, velocityLimit_);
	Vector3 result = object_->GetPosition() + gravity_ * velocity_;
	object_->SetPosition(result);
}