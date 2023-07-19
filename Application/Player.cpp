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
	moveAcc_(0.1f),			// 移動加速度
	maxSpd_(1.5f),// 最大速度
	moveSpd_(0.0f),
	cameraAngleSpd_(0.1f),	// カメラの角度の移動速度

	// クロスヘア
	crossHairHandle_(0),
	sCrossHair_(nullptr),

	// ジャンプ用
	isGround_(false),// 地面についているか
	gravity_(0.0f),// 重力
	gAcc_(0.2f),// 重力加速度
	jumpSpd_(2.5f)// ジャンプ速度
#pragma endregion
{
	// 入力クラスインスタンス取得
	key_	= Key::GetInstance();	// キーボード
	mouse_	= Mouse::GetInstance();	// マウス

	// カメラを生成＆設定
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 2.0f, 0.0f });

	// カメラを適用
	Object3D::SetCamera(camera_.get());

	// 弾のモデル読み込み
	mBullet_ = std::make_unique<Model>("sphere");

	// 弾にモデルを設定
	Bullet::SetModel(mBullet_.get());

	// レイのコライダーを生成
	eyeCollider_ = std::make_unique<RayCollider>(camera_->GetEye());
	eyeCollider_->SetAttribute(COL_PLAYER_RAY);
	eyeCollider_->SetIsCollision(false);

	// コライダーを追加
	CollisionManager::GetInstance()->AddCollider(eyeCollider_.get());

	// レイのコライダーを生成
	downCollider_ = std::make_unique<RayCollider>(camera_->GetEye());
	downCollider_->SetAttribute(COL_DOWN_RAY);
	downCollider_->SetIsCollision(true);
	downCollider_->SetDir(Vector3(0.0f, -1.0f, 0.0f));

	// コライダーを追加
	CollisionManager::GetInstance()->AddCollider(downCollider_.get());

	crossHairHandle_ = LoadTexture("Resources/crossHair.png");
	sCrossHair_ = std::make_unique<Sprite>();
	sCrossHair_->SetAnchorPoint({ 0.5f, 0.5f });
	sCrossHair_->SetPosition({
		WinAPI::GetInstance()->GetWidth() / 2.0f,
		WinAPI::GetInstance()->GetHeight() / 2.0f });
	sCrossHair_->SetSize({ 26, 26 });
}

Player::~Player()
{
	CollisionManager::GetInstance()->RemoveCollider(eyeCollider_.get());
}

void Player::Update()
{
	// ジャンプ処理
	if (isGround_ && key_->TriggerKey(DIK_SPACE)) {
		gravity_ = -jumpSpd_;
		isGround_ = false;
	}

	if (isGround_ == false) {
		gravity_ += gAcc_;// 重力加速度を加算
		camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	}

	float3 eye = camera_->GetEye();
	eye.y = Util::Clamp(eye.y, 1000.0f, 2.0f);

	if (eye.y <= 2.0f) isGround_ = true;

	camera_->SetEye(eye);

	// 弾を撃つ処理
	Shoot();

	// 視点移動
	EyeMove();

	// 移動
	Move();

	// コライダーの更新
	ColliderUpdate();

	if (eyeCollider_->GetIsHit()) {
		int num = 0;
	}

	// カメラの更新
	camera_->Update();
}

void Player::Draw()
{
	for (auto& bullets : bullets_) {
		bullets->Draw();
	}
}

void Player::Draw2D()
{
	sCrossHair_->Draw(crossHairHandle_);
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

	// マウスを左クリックしていなかったらこの後の処理を飛ばす
	if (mouse_->TriggerMouseButton(MouseButton::M_LEFT) == false) return;

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

void Player::ColliderUpdate()
{
	// レイの開始位置を設定
	eyeCollider_->SetOffset(camera_->GetEye());

	// レイの方向を設定
	eyeCollider_->SetDir(forwardVec_);

	downCollider_->SetOffset(camera_->GetEye());
}

void Player::OnCollision()
{
	if (downCollider_->GetIsHit()) {
		isGround_ = true;
	}
}