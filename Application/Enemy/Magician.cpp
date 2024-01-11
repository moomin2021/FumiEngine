#include "Magician.h"

#include "Util.h"
#include "CollisionAttribute.h"
#include "PipelineManager.h"

#include <imgui_impl_DX12.h>

CollisionManager* Magician::sColMgr_ = nullptr;
Model* Magician::sModel_ = nullptr;
Player* Magician::sPlayer_ = nullptr;
NavMesh* Magician::sNavMesh_ = nullptr;

Magician::~Magician()
{
	sColMgr_->RemoveCollider(cGroundJudgment_.get());
	sColMgr_->RemoveCollider(cEnemy2Player_.get());
	sColMgr_->RemoveCollider(cHit_.get());
}

void Magician::Initialize(const Vector3& inPos)
{
#pragma region オブジェクト
	object_ = std::make_unique<Object3D>(sModel_);
	object_->SetPosition(inPos);
#pragma endregion

#pragma region コライダー
	cHit_ = std::make_unique<AABBCollider>();
	cHit_->SetOffset({ 0.0f, 2.0f, 0.0f });
	cHit_->SetRadius({ 0.3f, 0.975f, 0.3f });
	cHit_->SetAttribute(COL_ENEMY);
	cHit_->SetObject3D(object_.get());
	sColMgr_->AddCollider(cHit_.get());

	cGroundJudgment_ = std::make_unique<RayCollider>();
	cGroundJudgment_->SetOffSet({ 0.0f, 2.0f, 0.0f });
	cGroundJudgment_->SetDir({ 0.0f, -1.0f, 0.0f });
	cGroundJudgment_->SetAttribute(COL_LEG);
	cGroundJudgment_->SetObject3D(object_.get());
	sColMgr_->AddCollider(cGroundJudgment_.get());

	cEnemy2Player_ = std::make_unique<RayCollider>();
	cEnemy2Player_->SetOffSet({ 0.0f, 1.7f, 0.0f });
	cEnemy2Player_->SetAttribute(COL_PLAYER2ENEMY_RAY);
	cEnemy2Player_->SetObject3D(object_.get());
	sColMgr_->AddCollider(cEnemy2Player_.get());
#pragma endregion

	//line_ = std::make_unique<Line3D>();
	//line_->Initialize(100);
	//line_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
}

void Magician::Update()
{
	// 弾の更新処理
	BulletUpdate();

	// 状態別更新処理
	(this->*stateTable[(size_t)state_])();

	// 重力処理
	Gravity();

	// 回転処理
	Rotate();

	// プレイヤーの方向にレイを飛ばす
	Vector3 enemy2Player = sPlayer_->GetPosition() - object_->GetPosition();
	enemy2Player.normalize();
	cEnemy2Player_->SetDir(enemy2Player);

	if (knockBackSpd_ > 0.0f)
	{
		object_->SetPosition(object_->GetPosition() + knockBackVec_ * knockBackSpd_);
		knockBackSpd_ -= 0.1f;
	}

	if (object_->GetPosition().y <= -50.0f) isAlive_ = false;
}

void Magician::Draw()
{
	PipelineManager::PreDraw("Object3D");
	object_->Draw();

	PipelineManager::PreDraw("Particle", D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	for (auto& it : bullet_) it->Draw();
}

void Magician::OnCollision()
{
	for (auto& it : bullet_) it->OnCollision();

	// 
	LookingPlayer();

	// 接地判定
	GroundingJudgment();

	if (cHit_->GetIsHit() && cHit_->GetHitCollider()->GetAttribute() == COL_PLAYER_SHOT)
	{
		velocity_ = 0.0f;
		knockBackVec_ = sPlayer_->GetDir();
		knockBackVec_.normalize();
		knockBackVec_.y = 1.0f;
		knockBackSpd_ = 0.5f;
		hp_ -= 1;

		if (hp_ <= 0) isAlive_ = false;

		hitTime_ = Util::GetTimrMSec();
	}

	if (cHit_->GetIsHit() && cHit_->GetHitCollider()->GetAttribute() == COL_STAGE_OBJ)
	{
		Jump();
	}

	if (cHit_->GetIsHit() && cHit_->GetHitCollider()->GetAttribute() == COL_PLAYER)
	{
		Vector3 result = sPlayer_->GetPosition() - object_->GetPosition();
		result.normalize();
		sPlayer_->SetKnock(result);
	}

	float elapsed = (Util::GetTimrMSec() - hitTime_) / 1000.0f;
	if (hitReactionDuringTime_ >= elapsed)
	{
		object_->SetColor({ 0.8f, 0.0f, 0.0f, 1.0f });
	}

	else
	{
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	if (state_ == State::CHASE) return;
}

void Magician::MatUpdate()
{
	object_->MatUpdate();
	for (auto& it : bullet_) it->MatUpdate();
	//line_->MatUpdate();
}

void Magician::Debug()
{
	ImGui::Begin("Enemy");
	ImGui::Text("angle = %f", angle_);
	ImGui::End();
}

void (Magician::* Magician::stateTable[]) () = {
	&Magician::Wait,		// 待機状態
	&Magician::Patrol,	// 見回り状態
	&Magician::Chase,		// 追跡状態
	&Magician::Magic,		// 魔法状態
};

void Magician::Wait()
{
}

void Magician::Patrol()
{

}

void Magician::Chase()
{
	// ルート探索
	CreateNavRoute();

	if (route_.size() == 0) return;
	Vector3 moveVec = route_[0] - object_->GetPosition();
	moveVec = { moveVec.x, 0.0f, moveVec.z };

	if (moveVec.length() < moveSpd_)
	{
		route_.erase(route_.begin());
	}

	moveVec.normalize();

	object_->SetPosition(object_->GetPosition() + moveVec * moveSpd_);
}

void Magician::Magic()
{
	// クールタイムが始まってからの経過時間
	float elapsedTime = (Util::GetTimrMSec() - startCoolDown_) / 1000.0f;

	// 経過時間がクールタイムより短かったら処理を飛ばす
	if (elapsedTime < coolDown_) return;

	// 自分からプレイヤーまでのベクトル
	Vector3 enemy2Player = (sPlayer_->GetPosition() + Vector3(0.0f, 1.0f, 0.0f)) - (object_->GetPosition() + Vector3(0.0f, 2.5f, 0.0f));
	enemy2Player.normalize();

	// 新しい弾の生成
	std::unique_ptr<MagicianBullet> newBullet = std::make_unique<MagicianBullet>();
	newBullet->Initialize(object_->GetPosition() + Vector3(0.0f, 2.5f, 0.0f), enemy2Player);
	bullet_.emplace_back(std::move(newBullet));
	startCoolDown_ = Util::GetTimrMSec();

	// ステータス変更
	state_ = State::CHASE;
}

void Magician::GroundingJudgment()
{
	// 接地フラグを[OFF]にする
	isGround_ = false;
	
	// 接地コライダーが衝突していなかったらこの後の処理を飛ばす
	if (cGroundJudgment_->GetIsHit() == false) return;

	// 衝突したコライダーまでの長さがゾンビの身長より長かったら処理を飛ばす
	if (cGroundJudgment_->GetDistance() > height_) return;

	// 押し戻し処理をする
	float reject = height_ - cGroundJudgment_->GetDistance();
	Vector3 result = object_->GetPosition() + Vector3{ 0.0f, reject, 0.0f };
	object_->SetPosition(result);
	isGround_ = true;
}

void Magician::Gravity()
{
	if (isGround_) return;
	velocity_ += accel_;
	velocity_ = Util::Min(velocity_, velocityLimit_);
	Vector3 result = object_->GetPosition() + Vector3{ 0.0f, -1.0f, 0.0f } *velocity_;
	object_->SetPosition(result);
}

void Magician::Rotate()
{
	// エネミーからプレイヤーまでの向きを計算
	Vector2 enemy = { object_->GetPosition().x, object_->GetPosition().z };
	Vector2 player = { 0.0f, 0.0f };
	if (route_.size() != 0)
	{
		player = { route_[0].x, route_[0].z };
	}

	else
	{
		player = { sPlayer_->GetPosition().x, sPlayer_->GetPosition().z };
	}
	Vector2 enemy2Player = player - enemy;
	enemy2Player.normalize();

	// エネミーの向きを計算
	forwardVec_ = { sinf(Util::Degree2Radian(angle_)), cosf(Util::Degree2Radian(angle_)) };
	forwardVec_.normalize();

	float result = Util::Radian2Degree(acosf(forwardVec_.dot(enemy2Player)));

	// 角度を計算
	if (forwardVec_.cross(enemy2Player) < 0)
	{
		if (result < turnSpd_) angle_ += result;
		else angle_ += turnSpd_;
	}

	else
	{
		if (result < turnSpd_) angle_ -= result;
		else angle_ -= turnSpd_;
	}

	object_->SetRotation({ 0.0f, angle_, 0.0f });

	//ImGui::Begin("Enemy");
	//ImGui::Text("enemy = {%f, %f}", enemy.x, enemy.y);
	//ImGui::Text("player = {%f, %f}", player.x, player.y);
	//ImGui::Text("enemy2player = {%f, %f}", enemy2Player.x, enemy2Player.y);
	//ImGui::Text("forwardVec = {%f, %f}", forwardVec_.x, forwardVec_.y);
	//ImGui::Text("resultAngle = %f", result);
	//ImGui::Text("cross = %f", forwardVec_.cross(enemy2Player));
	//ImGui::End();
}

void Magician::CreateNavRoute()
{
	// 最後にルート探索してからの経過時間
	float elapsedTime = (Util::GetTimrMSec() - lastRouteSearchTime_) / 1000.0f;

	// 経過時間がインターバルを過ぎていたらルート探索をする
	if (!(routeSearchInterval_ <= elapsedTime)) return;

	lastRouteSearchTime_ = Util::GetTimrMSec();

	Vector3 addVec = { 0.0f, 2.0f, 0.0f };

	bool result = sNavMesh_->RouteSearch(object_->GetPosition() + Vector3(0.0f, 1.0f, 0.0f), sPlayer_->GetPosition() + Vector3(0.0f, 1.0f, 0.0f), route_);

	// ルート探索がうまくいってなかったらこの後の処理を飛ばす
	if (!result) return;

	route_.erase(route_.begin());
}

void Magician::Jump()
{
	if (isGround_) velocity_ = JumpSpd_;
}

void Magician::LookingPlayer()
{
	// 自分からプレイヤーまでのベクトル
	Vector3 enemy2Player = sPlayer_->GetPosition() - object_->GetPosition();

	// プレイヤーと敵の間に障害物があったら処理を飛ばす
	if (cEnemy2Player_->GetDistance() < enemy2Player.length()) return;

	// プレイヤーが射程範囲外なら処理を飛ばす
	if (enemy2Player.length() > range_) return;
	
	// ステータスの変更
	state_ = State::MAGIC;
}

void Magician::BulletUpdate()
{
	for (auto it = bullet_.begin(); it != bullet_.end();)
	{
		// 敵の更新
		(*it)->Update();

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false)
		{
			it = bullet_.erase(it);
		}
		else ++it;
	}
}