#include "Player.h"
#include "Util.h"
#include "FumiEngine.h"
#include "WinAPI.h"

Player::Player() :
#pragma region 初期化リスト
	col_{},// -> 当たり判定用データ

	key_(nullptr),// -> キーボード入力
	mouse_(nullptr),// -> マウス
	camera_(nullptr),// -> カメラ

	// カメラアングル
	angleX_(0.0f), angleY_(0.0f),

	cameraSens_(0.0f),// -> カメラ感度

	moveSpeed_(0.0f),// -> 移動速度

	forwardVec_{0.0f, 0.0f, 0.0f},// -> 前方ベクトル
	rightVec_{0.0f, 0.0f, 0.0f}// -> 右ベクトル
#pragma endregion
{}

Player::~Player()
{
	delete reticle_;
}

void Player::Initialize()
{
	col_ = { {-10.0f, 0.0f }, 1.0f };// -> 当たり判定用データ
	oldCol_ = { {0.0f, 0.0f}, 1.0f };

	key_ = Key::GetInstance();// -> キーボード入力インスタンス取得
	mouse_ = Mouse::GetInstance();// -> マウス入力インスタンス取得

	// カメラアングル
	angleX_ = 0.0f;
	angleY_ = 90.0f;

	cameraSens_ = 0.05f;// -> カメラ感度

	moveSpeed_ = 0.4f;// -> 移動速度

	reticleH_ = LoadTexture("Resources/reticle.png");
	reticle_ = new Sprite();
	reticle_->position.x = WinAPI::GetWidth() / 2.0f;
	reticle_->position.y = WinAPI::GetHeight() / 2.0f;
}

void Player::Update()
{
	oldCol_ = col_;

	// 視点移動処理
	EyeMove();

	Move();

	col_.p.x = camera_->eye_.x;
	col_.p.y = camera_->eye_.z;

	reticle_->Update();
}

void Player::Draw()
{
	Sprite::PreDraw();
	reticle_->Draw(reticleH_);
}

void Player::EyeMove()
{
	angleX_ += mouse_->GetMouseVelosity().x * cameraSens_;
	angleY_ += mouse_->GetMouseVelosity().y * cameraSens_;

	// 上下の回転を制御する
	angleY_ = Util::Clamp(angleY_, 180.0f, 0.0f);

	// 前方ベクトルの計算
	forwardVec_.x = sinf(Util::Degree2Radian(angleX_));
	forwardVec_.y = cosf(Util::Degree2Radian(angleY_));
	forwardVec_.z = cosf(Util::Degree2Radian(angleX_));

	// 前方ベクトルの正規化
	forwardVec_.normalize();

	// 右ベクトルの計算
	rightVec_ = Vector3(forwardVec_.x, 0.0f, forwardVec_.z);
	rightVec_ = -rightVec_.cross(rightVec_ + Vector3(0.0f, 1.0f, 0.0f));

	// 右ベクトルの正規化
	rightVec_.normalize();

	// --注視点更新処理-- //
	camera_->target_.x = camera_->eye_.x + sinf(Util::Degree2Radian(angleX_)) * 10.0f;
	camera_->target_.y = camera_->eye_.y + cosf(Util::Degree2Radian(angleY_)) * 10.0f;
	camera_->target_.z = camera_->eye_.z + cosf(Util::Degree2Radian(angleX_)) * 10.0f;
}

void Player::Move()
{
	Vector3 forwardMove{};
	Vector3 rightMove{};

	// --スティックの入力方向で移動する-- //
	forwardMove.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * forwardVec_.z * moveSpeed_;
	forwardMove.x += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * forwardVec_.x * moveSpeed_;
	rightMove.z += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * rightVec_.z * moveSpeed_;
	rightMove.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * rightVec_.x * moveSpeed_;

	camera_->eye_.x += forwardMove.x + rightMove.x;
	camera_->eye_.z += forwardMove.z + rightMove.z;
	camera_->target_.x = camera_->eye_.x + sinf(Util::Degree2Radian(angleX_)) * 10.0f;
	camera_->target_.z = camera_->eye_.z + cosf(Util::Degree2Radian(angleX_)) * 10.0f;
}

void Player::Target() {
	camera_->target_.x = camera_->eye_.x + sinf(Util::Degree2Radian(angleX_)) * 10.0f;
	camera_->target_.z = camera_->eye_.z + cosf(Util::Degree2Radian(angleX_)) * 10.0f;
}
