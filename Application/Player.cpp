#include "Player.h"
#include "CollisionAttribute.h"
#include "Util.h"

#include <imgui_impl_DX12.h>

Player::Player()
{
}

Player::~Player()
{
	colMgr_->RemoveCollider(playerCol_.get());
	colMgr_->RemoveCollider(legCol_.get());
}

void Player::Initialize()
{
#pragma region インスタンス
	key_ = Key::GetInstance();
	mouse_ = Mouse::GetInstance();
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -10.0f });
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region モデル
	mSphere_ = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region オブジェクト
	oPlayer_ = std::make_unique<Object3D>(mSphere_.get());
#pragma endregion

#pragma region コライダー
	playerCol_ = std::make_unique<SphereCollider>();
	playerCol_->SetAttribute(COL_PLAYER);
	playerCol_->SetObject3D(oPlayer_.get());
	colMgr_->AddCollider(playerCol_.get());

	legCol_ = std::make_unique<RayCollider>();
	legCol_->SetAttribute(COL_LEG);
	legCol_->SetObject3D(oPlayer_.get());
	legCol_->SetDir({ 0.0f, -1.0f, 0.0f });
	colMgr_->AddCollider(legCol_.get());
#pragma endregion
}

void Player::Update()
{
	// 状態別更新処理
	(this->*stateTable[state_])();
}

void Player::Draw3D()
{

}

void Player::OnCollision()
{
	if (playerCol_->GetIsHit()) {
		Vector3 reject = playerCol_->GetReject();
		camera_->SetEye(camera_->GetEye() + reject);
		camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
		oPlayer_->SetPosition(camera_->GetEye());
	}

	if (legCol_->GetIsHit() && legCol_->GetDistance() <= 2.0f) {
		state_ = NORMAL;
		gravity_ = 0.0f;
		Vector3 reject = (2.0f - legCol_->GetDistance()) * Vector3(0.0f, 1.0f, 0.0f);
		camera_->SetEye(camera_->GetEye() + reject);
		camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
		oPlayer_->SetPosition(camera_->GetEye());
	}

	else {
		state_ = AIR;
	}
}

void Player::MatUpdate()
{
	// カメラ
	camera_->Update();

	// オブジェクト
	oPlayer_->MatUpdate();
}

void (Player::* Player::stateTable[]) () = {
	&Player::Normal,// 通常状態
	&Player::Air,	// 空中状態
	&Player::Climb,	// 登り状態
};

void Player::Normal()
{
	// 視点操作
	EyeMove();

	// 移動操作
	Move();

	// ジャンプ処理
	Jump();
}

void Player::Air()
{
	// 視点操作
	EyeMove();

	// 移動操作
	Move();

	// 落下処理
	Fall();
}

void Player::Climb()
{
}

void Player::Move()
{
	// 入力方向
	Vector3 inputVec = {
		static_cast<float>(key_->PushKey(DIK_D) - key_->PushKey(DIK_A)),
		0.0f,
		static_cast<float>(key_->PushKey(DIK_W) - key_->PushKey(DIK_S))
	};

	// 移動入力がされていたら加速させる
	if (inputVec.length() > 0.0f)	moveSpd_ += moveAcc_;
	else							moveSpd_ -= moveAcc_;

	// 速度制限
	if (isDash_) moveSpd_ = Util::Clamp(moveSpd_, dashSpd_, 0.0f);
	else moveSpd_ = Util::Clamp(moveSpd_, maxMoveSpd_, 0.0f);

	// 正面ベクトルの移動量
	Vector3 forwardMoveVec = {
		inputVec.z * forwardVec_.x,
		0.0f,
		inputVec.z * forwardVec_.z
	};

	// 右方向ベクトルの移動量
	Vector3 rightMoveVec = {
		inputVec.x * rightVec_.x,
		0.0f,
		inputVec.x * rightVec_.z
	};

	// 移動ベクトルの結果
	Vector3 resultVec = forwardMoveVec + rightMoveVec;
	resultVec.normalize();

	// カメラを更新
	camera_->SetEye(camera_->GetEye() + resultVec * moveSpd_);
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);

	// オブジェクトの位置を更新
	oPlayer_->SetPosition(camera_->GetEye());
}

void Player::EyeMove()
{
	// マウスの移動量分視点の角度に加算
	eyeAngle_.x += mouse_->GetMouseVelosity().x * sencivity_;
	eyeAngle_.y += mouse_->GetMouseVelosity().y * sencivity_;

	// 視点移動の上下に制限を付ける
	eyeAngle_.y = Util::Clamp(eyeAngle_.y, 120.0f, 60.0f);

	// 前方ベクトル計算
	forwardVec_ = {
		sinf(Util::Degree2Radian(eyeAngle_.x)),
		cosf(Util::Degree2Radian(eyeAngle_.y)),
		cosf(Util::Degree2Radian(eyeAngle_.x))
	};

	// 前方ベクトル正規化
	forwardVec_.normalize();

	// 右ベクトル計算
	rightVec_ = Vector3(forwardVec_.x, 0.0f, forwardVec_.z);
	rightVec_ = -rightVec_.cross(rightVec_ + Vector3(0.0f, 1.0f, 0.0f));

	// 右ベクトル計算
	rightVec_.normalize();

	// カメラ設定
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);

	// オブジェクトの位置を更新
	oPlayer_->SetPosition(camera_->GetEye());
}

void Player::Jump()
{
	// [SPACE]が押されたら上方向に加速させる
	if (key_->TriggerKey(DIK_SPACE)) {
		// 重力を更新
		gravity_ = -jumpSpd_;

		// カメラの位置を更新
		camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
		camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);

		// 衝突判定用のコライダーを更新
		oPlayer_->SetPosition(camera_->GetEye());
	}
}

void Player::Fall()
{
	// 重力加速度を加算
	gravity_ += gAcc_;// 重力加速度を加算

	// 最大重力を超えないように
	gravity_ = Util::Clamp(gravity_, maxGravity_, -1000.0f);

	// カメラを更新
	camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);

	// 衝突判定用のコライダーを更新
	oPlayer_->SetPosition(camera_->GetEye());
}