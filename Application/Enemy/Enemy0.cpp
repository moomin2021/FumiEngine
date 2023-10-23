#include "Enemy0.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "EnemyManager.h"
#include "PipelineManager.h"

Player* Enemy0::player_ = nullptr;
NavMesh* Enemy0::sNavMesh_ = nullptr;

Enemy0::Enemy0(Model* model) :
	// HP
	hp_(3),

	damageCounter_(0)
{
	// オブジェクト生成
	object_ = std::make_unique<Object3D>(model);

	// 線
	line_ = std::make_unique<Line3D>();
	line_->Initialize(100);
	line_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
}

Enemy0::~Enemy0()
{
	// コライダーを削除
	CollisionManager::GetInstance()->RemoveCollider(collider_.get());
}

void Enemy0::Initialize(Vector3 pos, Vector3 scale)
{
	// 初期位置設定
	object_->SetPosition(pos);
	object_->SetScale(scale);

	// コライダー生成
	collider_ = std::make_unique<SphereCollider>(Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f);

	// コライダーとオブジェクトを紐づけ
	collider_->SetObject3D(object_.get());

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

	// 状態別更新処理
	(this->*stateTable[state_])();

	// HPが0以下になったら生存フラグを[OFF]にする
	if (hp_ <= 0) isAlive_ = false;
}

void Enemy0::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// オブジェクト描画
	if (isAlive_) object_->Draw();

	PipelineManager::PreDraw("Line3D", D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// 線
	line_->Draw();

	PipelineManager::PreDraw("Object3D");
}

void (Enemy0::* Enemy0::stateTable[]) () = {
	&Enemy0::Wait,		// 待機状態
	&Enemy0::RandomMove,// ランダム移動状態
	&Enemy0::Chase,		// 追跡状態
};

void Enemy0::Wait()
{
	//// 経過時間
	//uint64_t elapsedTime = Util::GetTimeSec() - waitStartTime_;

	//// 待機時間開始が指定時間以上ならランダム移動状態にする
	//if (waitStartTime_ <= elapsedTime) {
	//	state_ = RANDOMMOVE;
	//	randomMoveVec_ = Vector3(
	//		Util::GetRandomFloat(-1.0f, 1.0f), 0.0f,
	//		Util::GetRandomFloat(-1.0f, 1.0f)).normalize();
	//	rndMoveStartTime_ = Util::GetTimeSec();
	//}

	//// プレイヤーとの距離
	//float dist = Vector3(object_->GetPosition() - player_->GetPosition()).length();

	//// プレイヤーが索敵範囲に入ったら追跡状態になる
	//if (searchRange_ >= dist) {
	//	state_ = CHASE;
	//	horizontalMoveStartTime_ = Util::GetTimeSec();
	//}
}

void Enemy0::RandomMove()
{
	// 経過時間
	uint64_t elapsedTime = Util::GetTimeSec() - rndMoveStartTime_;

	// 座標更新
	Vector3 pos = object_->GetPosition() + (randomMoveVec_ * rndMoveSpd_);
	object_->SetPosition(pos);

	// 経過時間が指定の時間をすぎたらランダム移動をやめる
	if (rndMoveTime_ <= elapsedTime) {
		state_ = WAIT;
		waitStartTime_ = Util::GetTimeSec();
	}

	// プレイヤーとの距離
	float dist = Vector3(object_->GetPosition() - player_->GetPosition()).length();

	// プレイヤーが索敵範囲に入ったら追跡状態になる
	if (searchRange_ >= dist) {
		state_ = CHASE;
		horizontalMoveStartTime_ = Util::GetTimeSec();
	}
}

void Enemy0::Chase()
{
	// 横移動経過時間
	uint64_t elapsedTime = Util::GetTimeSec() - horizontalMoveStartTime_;

	// エネミーからプレイヤーまでの方向
	Vector3 s = player_->GetPosition() - object_->GetPosition();
	Vector3 enemy2Player = Vector3(player_->GetPosition() - object_->GetPosition());
	enemy2Player.y = 0.0f;
	enemy2Player.normalize();

	// 右ベクトルの計算
	Vector3 rightVec = Vector3(enemy2Player.x, 0.0f, enemy2Player.z);
	rightVec = -rightVec.cross(rightVec + Vector3(0.0f, 1.0f, 0.0f));
	rightVec.normalize();

	// 横移動経過時間が指定時間を過ぎたら横移動方向を切り替える
	if (horizontalMoveSwitchTime_ <= elapsedTime) {
		isMoveRight_ *= -1.0f;
		horizontalMoveStartTime_ = Util::GetTimeSec();
	}

	Vector3 pos = object_->GetPosition() + (enemy2Player * frontRearMoveSpd_) + (rightVec * horizontalMoveSpd_ * isMoveRight_);
	object_->SetPosition(pos);

	// 弾を撃つ処理
	Shoot();
}

void Enemy0::OnCollision()
{
	// 衝突していなかったら処理を飛ばす
	if (collider_->GetIsHit() == false) return;
	if (collider_->GetHitCollider()->GetAttribute() != COL_PLAYER_SHOT) return;
	hp_ -= 1;
	object_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
	damageCounter_ = 0;
}

void Enemy0::MatUpdate()
{
	// オブジェクト更新
	object_->MatUpdate();

	// 線
	line_->MatUpdate();
}

void Enemy0::CreateNavRoute()
{
	Vector3 addVec = { 0.0f, 1.0f, 0.0f };

	// 線の削除
	line_->ClearPoint();

	sNavMesh_->RouteSearch(object_->GetPosition(), player_->GetPosition(), route_);

	for (uint16_t i = 0; i < route_.size() - 1; i++)
	{
		line_->AddPoint(route_[i] + addVec, route_[i + 1] + addVec);
	}
}

void Enemy0::Shoot()
{
	//// 撃ってからの経過時間
	//uint64_t elapsedTime = Util::GetTimeSec() - shootTime_;

	//// 経過時間が指定時間を過ぎたら撃つ
	//if (elapsedTime >= shootInterval_) {
	//	// 撃った時間を記録
	//	shootTime_ = Util::GetTimeSec();

	//	// 弾を生成
	//	EnemyManager::AddBullet(ENEMY0, object_->GetPosition(), player_->GetPosition() - object_->GetPosition());
	//}
}
