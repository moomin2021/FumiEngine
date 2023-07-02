#include "Player.h"
#include "Object3D.h"
#include "float3.h"
#include "Vector3.h"
#include "Util.h"

Player::Player() :
#pragma region 初期化リスト
	// 入力クラスインスタンス
	key_(nullptr),
	mouse_(nullptr),

	// 視点カメラ
	camera_(nullptr),

	// 弾のモデル
	mBullet_(nullptr),

	// 弾
	bullets_{},

	// 方向ベクトル
	forwardVec_{},	// 前方
	rightVec_{},	// 右

	// 速度
	moveSpd_(1.0f),// 移動速度
	cameraAngleSpd_(0.3f)// カメラの角度の移動速度
#pragma endregion
{
	// 入力クラスインスタンス取得
	key_	= Key::GetInstance();	// キーボード
	mouse_	= Mouse::GetInstance();	// マウス

	// カメラを生成＆設定
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// カメラを適用
	Object3D::SetCamera(camera_.get());

	// 弾のモデル読み込み
	mBullet_ = std::make_unique<Model>("sphere");

	// 弾にモデルを設定
	Bullet::SetModel(mBullet_.get());
}

void Player::Update()
{
	// 弾を撃つ処理
	Shoot();

	// 視点移動
	EyeMove();

	// 移動
	Move();

	// カメラの更新
	camera_->Update();
}

void Player::Draw()
{
	for (auto& bullets : bullets_) {
		bullets->Draw();
	}
}

void Player::Shoot()
{
	for (size_t i = 0; i < bullets_.size(); i++) {
		// 弾の更新処理
		bullets_[i]->Update();

		// 生存フラグが[OFF]だったら
		if (bullets_[i]->GetIsAlive() == false) {
			// 弾を消す
			bullets_.erase(bullets_.begin() + i);
		}
	}

	// マウスを左クリックしていなかったらこの後の処理を飛ばす
	if (mouse_->TriggerMouseButton(MouseButton::M_LEFT) == false) return;

	// 弾を生成
	bullets_.emplace_back(std::make_unique<Bullet>(camera_->GetEye(), forwardVec_));
}

void Player::EyeMove()
{
	// 視点の角度
	static float3 angle = { 0.0f, 90.0f, 0.0f };

	// マウスの移動量分視点の角度に加算
	angle.x += mouse_->GetMouseVelosity().x * cameraAngleSpd_;
	angle.y += mouse_->GetMouseVelosity().y * cameraAngleSpd_;

	// 視点移動の上下に制限を付ける
	angle.y = Util::Clamp(angle.y, 180.0f, 0.0f);

	// 前方ベクトル計算
	forwardVec_ = {
		sinf(Util::Degree2Radian(angle.x)),
		cosf(Util::Degree2Radian(angle.y)),
		cosf(Util::Degree2Radian(angle.x))
	};

	// 前方ベクトル正規化
	forwardVec_.normalize();

	// 右ベクトルの計算
	rightVec_ = Vector3(forwardVec_.x, 0.0f, forwardVec_.z);
	rightVec_ = -rightVec_.cross(rightVec_ + Vector3(0.0f, 1.0f, 0.0f));

	// 右ベクトル正規化
	rightVec_.normalize();

	// カメラの注視点を設定
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
}

void Player::Move()
{
	// 移動方向
	Vector3 moveVec = {
		static_cast<float>(key_->PushKey(DIK_D) - key_->PushKey(DIK_A)),
		0.0f,
		static_cast<float>(key_->PushKey(DIK_W) - key_->PushKey(DIK_S))
	};

	float3 forwardMove = {
		moveVec.z * forwardVec_.x * moveSpd_,
		0.0f,
		moveVec.z * forwardVec_.z * moveSpd_
	};

	float3 rightMove = {
		moveVec.x * rightVec_.x * moveSpd_,
		0.0f,
		moveVec.x * rightVec_.z * moveSpd_
	};

	camera_->SetEye(camera_->GetEye() + (forwardMove + rightMove));

	// カメラの注視点を設定
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
}