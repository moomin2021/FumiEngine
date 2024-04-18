#include "Zombie.h"
#include "Player.h"

#include "Util.h"
#include "CollisionAttribute.h"

#include <imgui_impl_DX12.h>

void Zombie::Initialize(const Vector3& inPos)
{
	nowHP_ = 3;

	object_ = std::make_unique<Object3D>(model0_);
	object_->SetPosition(inPos);

	headColData_.offset = { 0.0f, 1.75f, 0.0f };
	headColData_.radius = { 0.25f, 0.25f, 0.25f };
	headC_ = std::make_unique<AABBCollider>();
	headC_->SetOffset(headColData_.offset);
	headC_->SetRadius(headColData_.radius);
	headC_->SetAttribute(COL_ENEMY_HEAD);
	headC_->SetObject3D(object_.get());
	colMgr_->AddCollider(headC_.get());

	bodyColData_.offset = { 0.0f, 0.75f, 0.0f };
	bodyColData_.radius = { 0.25f, 0.75f, 0.25f };
	bodyC_ = std::make_unique<AABBCollider>();
	bodyC_->SetOffset(bodyColData_.offset);
	bodyC_->SetRadius(bodyColData_.radius);
	bodyC_->SetAttribute(COL_ENEMY_BODY);
	bodyC_->SetObject3D(object_.get());
	colMgr_->AddCollider(bodyC_.get());

	groundColData_.offset = { 0.0f, 1.75f, 0.0f };
	groundColData_.rayDir = { 0.0f, -1.0f, 0.0f };
	groundC_ = std::make_unique<RayCollider>();
	groundC_->SetOffSet(groundColData_.offset);
	groundC_->SetDir(groundColData_.rayDir);
	groundC_->SetPushBackDistance(groundColData_.offset.y);
	groundC_->SetAttribute(COL_LEG);
	groundC_->SetObject3D(object_.get());
	colMgr_->AddPushBackRayCollider(groundC_.get());
}

void Zombie::Update()
{
	if (DeltaTime::GetInstance()->GetTimeSpeed() == 0.0f) return;

	// 状態別更新処理
	(this->*stateTable[(size_t)state_])();

	Vector3 pos = object_->GetPosition();

	ImGui::Begin("Zombie");

	ImGui::Text("POS = { %f, %f, %f }", pos.x, pos.y, pos.z);
	ImGui::Text("HP = %d", nowHP_);

	ImGui::End();
}

void Zombie::Draw()
{
	object_->Draw();
}

void Zombie::Collision()
{
	HeadHit();
	BodyHit();
	JudDeath();
	GroundingJudgment();
}

void Zombie::MatUpdate()
{
	object_->MatUpdate();
}

void Zombie::Finalize()
{
	colMgr_->RemoveCollider(headC_.get());
	colMgr_->RemoveCollider(bodyC_.get());
	colMgr_->RemovePushBackRayCollider(groundC_.get());
}

void (Zombie::* Zombie::stateTable[]) () = {
	&Zombie::Wait,
	&Zombie::Chase,
};

void Zombie::Wait()
{
	Rotate();
	Gravity();
}

void Zombie::Chase()
{
	Rotate();
	Gravity();
}

void Zombie::Rotate()
{
	// ルートがないなら処理を飛ばす
	//if (route_.size() == 0) return;

	Vector2 enemy = { object_->GetPosition().x, object_->GetPosition().z };
	Vector2 target = { pPlayer_->GetPosition().x, pPlayer_->GetPosition().z };
	Vector2 targetVec = target - enemy;

	// 現在の正面ベクトルを求める
	Vector2 nowForwardVec = 
		{ sinf(Util::Degree2Radian(angle_)), cosf(Util::Degree2Radian(angle_)) };

	// 各ベクトルを正規化
	targetVec.normalize();
	nowForwardVec.normalize();

	// ベクトルの差分を計算
	float difAngle = Util::Radian2Degree(acosf(nowForwardVec.dot(targetVec)));

	// どっち周りするか決める
	if (nowForwardVec.cross(targetVec) < 0)
	{
		if (difAngle < rotaSpd_) angle_ += difAngle;
		else angle_ += rotaSpd_;
	}

	else if (nowForwardVec.cross(targetVec) > 0)
	{
		if (difAngle < rotaSpd_) angle_ -= difAngle;
		else angle_ -= rotaSpd_;
	}


	// 回転
	Util::NormalizeAngle(angle_);
	object_->SetRotation({ 0.0f, angle_, 0.0f });
}

void Zombie::Gravity()
{
	// 接地していたら処理を飛ばす
	if (isGround_) return;

	// 重力の計算
	gravity_ += accel_;
	gravity_ = Util::Min(gravity_, gravityLimit_);

	// 座標の更新
	Vector3 result = object_->GetPosition() + gravityVec_ * gravity_;
	object_->SetPosition(result);
}

void Zombie::GroundingJudgment()
{
	// 接地しているか判定
	if (groundC_->GetIsHit()) return;
	if (groundC_->GetDistance() > headColData_.offset.y)
	{
		isGround_ = false;
	}

	else
	{
		isGround_ = true;
		gravity_ = 0.0f;
	}
}