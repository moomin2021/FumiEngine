#include "Player.h"
#include "Object3D.h"
#include "float3.h"
#include "Vector3.h"
#include "Util.h"
#include "Texture.h"
#include "WinAPI.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Util.h"

Player::Player()
{
#pragma region 入力クラス
	// 入力クラスインスタンス取得
	key_	= Key::GetInstance();	// キーボード
	mouse_	= Mouse::GetInstance();	// マウス
#pragma endregion

#pragma region カメラ
	// カメラを生成＆設定
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 2.0f, 0.0f });

	// カメラを適用
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region 弾
	// 弾のモデル読み込み
	mBullet_ = std::make_unique<Model>("sphere");

	// 弾にモデルを設定
	Bullet::SetModel(mBullet_.get());
#pragma endregion

#pragma region コライダー
	// レイのコライダーを生成
	eyeCollider_ = std::make_unique<RayCollider>(camera_->GetEye());
	eyeCollider_->SetAttribute(COL_PLAYER_RAY);
	eyeCollider_->SetIsCollision(false);

	// コライダーを追加
	CollisionManager::GetInstance()->AddCollider(eyeCollider_.get());
#pragma endregion

#pragma region クロスヘア
	crossHairHandle_ = LoadTexture("Resources/crossHair.png");
	sCrossHair_ = std::make_unique<Sprite>();
	sCrossHair_->SetAnchorPoint({ 0.5f, 0.5f });
	sCrossHair_->SetPosition({
		WinAPI::GetInstance()->GetWidth() / 2.0f,
		WinAPI::GetInstance()->GetHeight() / 2.0f });
	sCrossHair_->SetSize({ 26, 26 });
#pragma endregion
}

Player::~Player()
{
	CollisionManager::GetInstance()->RemoveCollider(eyeCollider_.get());
}

void Player::Update()
{
	// 状態別更新処理
	(this->*stateTable[state_])();

	// カメラの更新
	camera_->Update();
}

void Player::Object3DDraw()
{
	for (auto& bullets : bullets_) {
		bullets->Draw();
	}
}

void Player::FrontSpriteDraw()
{
	sCrossHair_->Draw(crossHairHandle_);
}

void (Player::* Player::stateTable[]) () = {
	&Player::Normal,// 通常状態
	&Player::Air,	// 空中状態
};

void Player::Normal() {
	// 視点操作
	EyeMove();

	// 移動操作
	Move();

	// 撃つ処理
	Shoot();

	// ジャンプ処理
	Jump();

	// コライダーの更新
	ColliderUpdate();
}

void Player::Air() {
	// 視点操作
	EyeMove();

	// 移動操作
	Move();

	// 撃つ処理
	Shoot();

	// 落下処理
	Fall();

	// コライダーの更新
	ColliderUpdate();
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

	eyeCollider_->SetIsCollision(false);

	// 残弾数が0以下ならこの後の処理を飛ばす
	if (nowBullet_ <= 0) return;

	// マウスを左クリックしていなかったらこの後の処理を飛ばす
	if (mouse_->TriggerMouseButton(MouseButton::M_LEFT) == false) return;

	// 残弾数を減らす
	nowBullet_--;

	eyeCollider_->SetIsCollision(true);

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

	if (moveVec.length() > 0.0f) {
		moveSpd_ += moveAcc_;
	}

	else {
		moveSpd_ -= moveAcc_;
	}

	moveSpd_ = Util::Clamp(moveSpd_, maxSpd_, 0.0f);

	float3 forwardMove = {
		moveVec.z * forwardVec_.x,
		0.0f,
		moveVec.z * forwardVec_.z
	};

	float3 rightMove = {
		moveVec.x * rightVec_.x,
		0.0f,
		moveVec.x * rightVec_.z
	};

	Vector3 resultVec = forwardMove + rightMove;
	resultVec.normalize();

	camera_->SetEye(camera_->GetEye() + resultVec * moveSpd_);

	// カメラの注視点を設定
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
}

void Player::Jump()
{
	// [SPACE]キーを押したらジャンプする
	if (isGround_ && key_->TriggerKey(DIK_SPACE)) {
		gravity_ = -jumpSpd_;
		isGround_ = false;
		state_ = AIR;
	}
}

void Player::Fall()
{
	// 地面に着いていないなら落下処理をする
	if (isGround_ == false) {
		gravity_ += gAcc_;// 重力加速度を加算
		camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	}

	// カメラの座標が2.0f以下にならないように
	float3 eye = camera_->GetEye();
	eye.y = Util::Clamp(eye.y, 1000.0f, 2.0f);

	// カメラの座標が2.0以下になったら地面に着いたことにする
	if (eye.y <= 2.0f) {
		isGround_ = true;
		state_ = NORMAL;
	}

	// カメラの座標更新
	camera_->SetEye(eye);

	// カメラの注視点を設定
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
}

void Player::Reload()
{
	// [R]キーが押されていなかったらこの後の処理を飛ばす
	if (key_->TriggerKey(DIK_R) == false) return;


}

void Player::ColliderUpdate()
{
	// レイの開始位置を設定
	eyeCollider_->SetOffset(camera_->GetEye());

	// レイの方向を設定
	eyeCollider_->SetDir(forwardVec_);
}

void Player::OnCollision()
{

}