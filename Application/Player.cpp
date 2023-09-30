#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
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
	camera_->SetEye({ 0.0f, 0.0f, -10.0f });
	Object3D::SetCamera(camera_.get());
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
}

void Player::MatUpdate()
{
	// カメラ
	camera_->Update();
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
}

void Player::Air()
{
}

void Player::Climb()
{
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
}