#include "Zombie.h"
#include "Player.h"

#include "Util.h"
#include "CollisionAttribute.h"
#include "PipelineManager.h"
#include "DeltaTime.h"

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

	line_ = std::make_unique<Line3D>();
	line_->Initialize(200);
	line_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
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
	PipelineManager::PreDraw("Line3D", D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	if (isDebug_)
	{
		line_->Draw();
	}
	PipelineManager::PreDraw("Object3D");
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
	line_->MatUpdate();
}

void Zombie::Finalize()
{
	colMgr_->RemoveCollider(headC_.get());
	colMgr_->RemoveCollider(bodyC_.get());
	colMgr_->RemovePushBackRayCollider(groundC_.get());
}

void Zombie::Debug(bool isDebug)
{
	isDebug_ = isDebug;
}

void (Zombie::* Zombie::stateTable[]) () = {
	&Zombie::Wait,
	&Zombie::Chase,
};

void Zombie::Wait()
{
	RandomRoute();
	Move();
	Rotate();
	Gravity();
}

void Zombie::Chase()
{
	Move();
	Rotate();
	Gravity();
}

void Zombie::Move()
{
	if (route_.size() == 0) return;

	moveVec_ = route_[0] - object_->GetPosition();
	moveVec_.y = 0.0f;

	if (moveVec_.length() < speed_) route_.erase(route_.begin());
	moveVec_.normalize();

	Vector3 result = object_->GetPosition() + moveVec_ * speed_;
	object_->SetPosition(result);
}

void Zombie::Rotate()
{
	// ルートがないなら処理を飛ばす
	if (route_.size() == 0) return;

	Vector2 enemy = { object_->GetPosition().x, object_->GetPosition().z };
	Vector2 target = { route_[0].x, route_[0].z };
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

bool Zombie::CreateRoute(const Vector3& goal)
{
	Vector3 offset = { 0.0f, 1.0f, 0.0f };
	Vector3 start = object_->GetPosition();
	bool result = pNavMesh_->RouteSearch(start + offset, goal + offset, route_);

	if (result == false) return false;

	line_->ClearPoint();
	for (uint16_t i = 0; i < route_.size() - 1; i++) line_->AddPoint(route_[i] + offset, route_[i + 1] + offset);
	route_.erase(route_.begin());

	return true;
}

void Zombie::RandomRoute()
{
	static float elapsedTime = routeSearchInterval_;
	elapsedTime += DeltaTime::GetInstance()->GetDeltaTime();

	if (elapsedTime >= routeSearchInterval_)
	{
		Vector3 goal = Vector3();
		Vector3 pos = object_->GetPosition();
		goal.x = Util::GetRandomFloat(pos.x - randomRange_, pos.x + randomRange_);
		goal.y = 1.0f;
		goal.z = Util::GetRandomFloat(pos.z - randomRange_, pos.z + randomRange_);

		bool result = CreateRoute(goal);
		if (result) elapsedTime = 0.0f;
	}
}