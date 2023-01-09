#include "Player.h"
#include "Util.h"

Player::Player() :
#pragma region 初期化リスト
	col_{},// -> 当たり判定用データ
	oldCol_{},

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
	delete object_;
	delete model_;
}

void Player::Initialize()
{
	col_ = { {-10.0f, 0.0f }, 1.0f };// -> 当たり判定用データ
	oldCol_ = { {0.0f, 0.0f }, 1.0f};

	key_ = Key::GetInstance();// -> キーボード入力インスタンス取得
	mouse_ = Mouse::GetInstance();// -> マウス入力インスタンス取得

	model_ = Model::CreateModel("player_rest");
	object_ = Object3D::CreateObject3D();
	object_->SetCamera(camera_);
	object_->SetModel(model_);
	object_->position_.x = -10.0f;

	// カメラアングル
	angleX_ = 0.0f;
	angleY_ = 90.0f;

	cameraSens_ = 0.3f;// -> カメラ感度

	moveSpeed_ = 0.4f;// -> 移動速度
}

void Player::Update()
{
	oldCol_ = col_;

	// 視点移動処理
	EyeMove();

	Move();

	object_->position_.x = col_.p.x;
	object_->position_.z = col_.p.y;
}

void Player::Draw()
{
	object_->Draw();
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

	//camera_->eye_.x += forwardMove.x + rightMove.x;
	//camera_->eye_.z += forwardMove.z + rightMove.z;
	camera_->target_.x = camera_->eye_.x + sinf(Util::Degree2Radian(angleX_)) * 10.0f;
	camera_->target_.z = camera_->eye_.z + cosf(Util::Degree2Radian(angleX_)) * 10.0f;

	// 当たり判定を更新
	//col_.pos.x = camera_->eye_.x;
	//col_.pos.y = camera_->eye_.z;
	col_.p.x += forwardMove.x + rightMove.x;
	//camera_->eye_.y += forwardMove.y + rightMove.y;
	col_.p.y += forwardMove.z + rightMove.z;
}
