#include "Zombie.h"

#include "PipelineManager.h"

#include "CollisionAttribute.h"

CollisionManager3D* Zombie::sColMgr_ = nullptr;
Model* Zombie::sModel0_ = nullptr;
Model* Zombie::sModel1_ = nullptr;
Player* Zombie::sPlayer_ = nullptr;
NavMesh* Zombie::sNavMesh_ = nullptr;

Zombie::~Zombie()
{
	sColMgr_->RemoveCollider(headC_.get());
	sColMgr_->RemoveCollider(bodyC_.get());
	sColMgr_->RemovePushBackRayCollider(legC_.get());
}

void Zombie::Initialize(const Vector3& inPos, const Vector3& inWanderingPos, State inState)
{
	wanderingPos_ = inWanderingPos;
	state_ = inState;

#pragma region オブジェクト
	if (inState == State::CHASE)
	{
		object_ = std::make_unique<Object3D>(sModel1_);
		object_->SetPosition(inPos);
		speed_ = rushSpd_;
	}

	else
	{
		object_ = std::make_unique<Object3D>(sModel0_);
		object_->SetPosition(inPos);
		speed_ = defSpd_;
	}
#pragma endregion

#pragma region コライダー
	headC_ = std::make_unique<AABBCollider>();
	headC_->SetOffset(headOffset_);
	headC_->SetRadius(headRadius_);
	headC_->SetAttribute(COL_ENEMY_HEAD);
	headC_->SetObject3D(object_.get());
	sColMgr_->AddCollider(headC_.get());

	bodyC_ = std::make_unique<AABBCollider>();
	bodyC_->SetOffset(bodyOffset_);
	bodyC_->SetRadius(bodyRadius_);
	bodyC_->SetAttribute(COL_ENEMY_BODY);
	bodyC_->SetObject3D(object_.get());
	sColMgr_->AddCollider(bodyC_.get());

	legC_ = std::make_unique<RayCollider>();
	legC_->SetOffSet(headOffset_);
	legC_->SetDir({ 0.0f, -1.0f, 0.0f });
	legC_->SetPushBackDistance(headOffset_.y);
	legC_->SetAttribute(COL_LEG);
	legC_->SetObject3D(object_.get());
	sColMgr_->AddPushBackRayCollider(legC_.get());
#pragma endregion

	line_ = std::make_unique<Line3D>();
	line_->Initialize(100);
	line_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	waitT_.coolTime = 3.0f;
}

void Zombie::Update()
{
	// 状態別更新処理
	(this->*stateTable[(size_t)state_])();

	// 重力処理
	Gravity();
}

void Zombie::Draw()
{
	object_->Draw();
	PipelineManager::PreDraw("Line3D", D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	if (isDebug_) line_->Draw();
	PipelineManager::PreDraw("Object3D");
}

void Zombie::OnCollision()
{
	// 接地判定
	GroundingJudgment();

	// 攻撃を食らったときの処理
	Hit();
}

void Zombie::MatUpdate()
{
	object_->MatUpdate();
	line_->MatUpdate();
}

void Zombie::Debug(bool isDebug)
{
	if (isDebug == false) return;
}

void (Zombie::* Zombie::stateTable[]) () = {
	&Zombie::Wait,		// 待機状態
	&Zombie::Wandering,	// 徘徊状態
	&Zombie::Chase,		// 追跡状態
};

void Zombie::Wait()
{
	if (waitT_.GetOn())
	{
		while (true)
		{
			Vector3 offset = Vector3();
			offset.x = Util::GetRandomFloat(wanderingPos_.x - wanderingRadius_, wanderingPos_.x + wanderingRadius_);
			offset.y = 3.0f;
			offset.z = Util::GetRandomFloat(wanderingPos_.z - wanderingRadius_, wanderingPos_.z + wanderingRadius_);
			bool result = CreateRoute(offset);
			if (result) break;
		}
		state_ = State::WANDERING;
	}
}

void Zombie::Wandering()
{
	bool result = Move();
	if (result == false)
	{
		waitT_.last = Util::GetTimrMSec();
		state_ = State::WAIT;
	}

	// 回転処理
	Rotate();
}

void Zombie::Chase()
{
	speed_ += 0.01f;
	speed_ = Util::Min(speed_, rushSpd_);

	// ルート探索
	CreateRoute(sPlayer_->GetPosition());

	// 移動処理
	RushMove();

	// 回転処理
	Rotate();
}

bool Zombie::CreateRoute(const Vector3& goalPos)
{
	//// 最後にルート探索してからの経過時間
	//float elapsedTime = (Util::GetTimrMSec() - lastRouteSearchTime_) / 1000.0f;

	//// 経過時間がインターバルを過ぎていたらルート探索をする
	//if (!(routeSearchInterval_ <= elapsedTime)) return false;

	//// 最後にルート探索した時間を更新
	//lastRouteSearchTime_ = Util::GetTimrMSec();

	// ルート探索する際に加算するベクトル
	Vector3 addVec = { 0.0f, 1.0f, 0.0f };

	// ルートを検索
	bool result = sNavMesh_->RouteSearch(object_->GetPosition() + addVec, goalPos + addVec, route_);

	// ルート検索が失敗したら処理を飛ばす
	if (result == false) return false;

	// ルート描画用変数の初期化
	line_->ClearPoint();

	// ルート描画用に座標を設定
	for (uint16_t i = 0; i < route_.size() - 1; i++)
	{
		line_->AddPoint(route_[i], route_[i + 1]);
	}

	// 最初の座標を削除
	route_.erase(route_.begin());

	return true;
}

bool Zombie::Move()
{
	// ルートがなかったら処理を飛ばす
	if (route_.size() == 0) return false;

	// 移動方向を計算
	Vector3 moveVec = route_[0] - object_->GetPosition();
	moveVec = { moveVec.x, 0.0f, moveVec.z };

	// ルートの通過点を通ったら点を消す
	if (moveVec.length() < speed_) route_.erase(route_.begin());

	// 移動方向ベクトルを正規化
	moveVec.normalize();

	// オブジェクトの座標を更新
	object_->SetPosition(object_->GetPosition() + moveVec * speed_);

	return true;
}

bool Zombie::RushMove()
{
	Vector3 resultMoveVec = { moveVec_.x, 0.0f, moveVec_.z };
	resultMoveVec.normalize();

	if (route_.size() != 0)
	{
		Vector3 routeVec = route_[0] - object_->GetPosition();
		if (routeVec.length() < speed_) route_.erase(route_.begin());
		resultMoveVec += routeVec;
	}

	resultMoveVec.normalize();

	// オブジェクトの座標を更新
	object_->SetPosition(object_->GetPosition() + resultMoveVec * speed_);

	resultMoveVec = Vector3();

	return true;
}

void Zombie::GroundingJudgment()
{
	// 衝突していなかったら処理を飛ばす
	if (legC_->GetIsHit()) return;

	if (legC_->GetDistance() > headOffset_.y) return;

	isGround_ = true;
}

void Zombie::Gravity()
{
	// 接地していたら処理を飛ばす
	if (isGround_) return;

	// 速度を加速させる
	velocity_ += accel_;

	// 速度制限処理
	velocity_ = Util::Min(velocity_, velocityLimit_);

	// 重力方向
	Vector3 gravityVec = { 0.0f, -1.0f, 0.0f };

	// 重力を加算した後の座標を計算
	Vector3 result = object_->GetPosition() + gravityVec * velocity_;

	// オブジェクト座標更新
	object_->SetPosition(result);
}

void Zombie::Rotate()
{
	// ルートがなかったら処理を飛ばす
	if (route_.size() == 0) return;

	// エネミーからプレイヤーまでの向きを計算
	Vector2 enemy = { object_->GetPosition().x, object_->GetPosition().z };
	Vector2 player = { route_[0].x, route_[0].z };
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

	else if (forwardVec_.cross(enemy2Player) > 0)
	{
		if (result < turnSpd_) angle_ -= result;
		else angle_ -= turnSpd_;
	}

	object_->SetRotation({ 0.0f, angle_, 0.0f });
}

void Zombie::Hit()
{
	isBody_ = false;

	// 頭に攻撃を受けたら
	if (headC_->GetIsHit() && headC_->GetHitCollider()->GetAttribute() == COL_PLAYER_SHOT)
	{
		isAlive_ = false;
		isHead_ = true;
		hp_ = 0;
	}

	// 体に攻撃を受けたら
	if (bodyC_->GetIsHit() && bodyC_->GetHitCollider()->GetAttribute() == COL_PLAYER_SHOT)
	{
		velocity_ = 0.0f;
		knockBackVec_ = sPlayer_->GetDir();
		knockBackVec_.normalize();
		knockBackVec_.y = 1.0f;
		knockBackSpd_ = 0.5f;
		hp_ -= 1;
		speed_ = 0.0f;
		if (state_ != State::CHASE)
		{
			isBody_ = true;
		}

		if (hp_ <= 0) isAlive_ = false;

		hitTime_ = Util::GetTimrMSec();
	}

	// 壁に衝突したときの処理
	if (bodyC_->GetIsHit() && bodyC_->GetHitCollider()->GetAttribute() == COL_BLOCK)
	{
		Jump();
	}

	// ヒット時色を変更
	float elapsed = (Util::GetTimrMSec() - hitTime_) / 1000.0f;
	if (hitReactionDuringTime_ >= elapsed)
	{
		object_->SetColor({ 0.8f, 0.0f, 0.0f, 1.0f });
	}

	else
	{
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void Zombie::Jump()
{
	// 接地していなかったら処理を飛ばす
	if (isGround_ == false) return;

	// 速度を加算
	velocity_ = JumpSpd_;

	isGround_ = false;
}

void Zombie::SetRushMode()
{
	state_ = State::CHASE;
	object_->SetModel(sModel1_);
	speed_ = rushSpd_;
}