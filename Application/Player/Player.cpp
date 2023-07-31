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
#include "WinAPI.h"
#include "ParticleManager.h"

Player::Player()
{
	// ウィンドウサイズを取得
	float2 winSize = {
		static_cast<float>(WinAPI::GetInstance()->GetWidth()),
		static_cast<float>(WinAPI::GetInstance()->GetHeight()) };

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

	ParticleManager::SetCamera(camera_.get());
#pragma endregion

#pragma region 数字
	// 配列のサイズを指定
	numberHandle_.resize(10);

	// 画像を読み込み
	numberHandle_[0] = LoadTexture("Resources/number0.png");
	numberHandle_[1] = LoadTexture("Resources/number1.png");
	numberHandle_[2] = LoadTexture("Resources/number2.png");
	numberHandle_[3] = LoadTexture("Resources/number3.png");
	numberHandle_[4] = LoadTexture("Resources/number4.png");
	numberHandle_[5] = LoadTexture("Resources/number5.png");
	numberHandle_[6] = LoadTexture("Resources/number6.png");
	numberHandle_[7] = LoadTexture("Resources/number7.png");
	numberHandle_[8] = LoadTexture("Resources/number8.png");
	numberHandle_[9] = LoadTexture("Resources/number9.png");
#pragma endregion

#pragma region 弾
	// 弾のモデル読み込み
	mBullet_ = std::make_unique<Model>("sphere");

	// 残弾数表示UIを読み込み
	bulletValueDisplayFrameHandle_ = LoadTexture("Resources/BulletValueDisplayFrame.png");

	// 残弾数表示UI用のスプライトを生成
	sBulletValueDisplayFrame_ = std::make_unique<Sprite>();
	sBulletValueDisplayFrame_->SetAnchorPoint({ 0.5f, 0.5f });
	sBulletValueDisplayFrame_->SetSize({ 300.0f, 140.0f });
	sBulletValueDisplayFrame_->SetPosition({ winSize.x - 150.0f, winSize.y - 70.0f });

	// 最大弾数表示用スプライト生成
	sMaxBulletUI_.resize(2);
	sMaxBulletUI_[0] = std::make_unique<Sprite>();
	sMaxBulletUI_[1] = std::make_unique<Sprite>();
	sMaxBulletUI_[0]->SetAnchorPoint({ 1.0f, 1.0f });
	sMaxBulletUI_[1]->SetAnchorPoint({ 1.0f, 1.0f });
	sMaxBulletUI_[0]->SetSize({ 35.25f, 54.0f });
	sMaxBulletUI_[1]->SetSize({ 35.25f, 54.0f });
	sMaxBulletUI_[0]->SetPosition({winSize.x - 85.75f, winSize.y - 30.0f});
	sMaxBulletUI_[1]->SetPosition({winSize.x - 45.0f, winSize.y - 30.0f});

	// 残弾数表示スプライト
	sNowBulletUI_.resize(2);
	sNowBulletUI_[0] = std::make_unique<Sprite>();
	sNowBulletUI_[1] = std::make_unique<Sprite>();
	sNowBulletUI_[0]->SetAnchorPoint({ 0.5f, 1.0f });
	sNowBulletUI_[1]->SetAnchorPoint({ 0.5f, 1.0f });
	sNowBulletUI_[0]->SetSize({ 47.0f, 72.0f });
	sNowBulletUI_[1]->SetSize({ 47.0f, 72.0f });
	sNowBulletUI_[0]->SetPosition({ winSize.x - 247.0f, winSize.y - 30.0f });
	sNowBulletUI_[1]->SetPosition({ winSize.x - 195.0f, winSize.y - 30.0f });
#pragma endregion

#pragma region コライダー
	// レイのコライダーを生成
	eyeCollider_ = std::make_unique<RayCollider>(camera_->GetEye());
	eyeCollider_->SetAttribute(COL_PLAYER_RAY);
	eyeCollider_->SetIsCollision(false);

	// コライダーを追加
	CollisionManager::GetInstance()->AddCollider(eyeCollider_.get());

	// 球のコライダー生成
	playerCollider_ = std::make_unique<SphereCollider>(camera_->GetEye() + float3(0.0f, 1.0f, 0.0f));
	playerCollider_->SetRadius(2.0f);
	playerCollider_->SetAttribute(COL_PLAYER);

	// コライダーを追加
	CollisionManager::GetInstance()->AddCollider(playerCollider_.get());
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

#pragma region リロード
	// 画像読み込み
	reloadUIHandle_ = LoadTexture("Resources/reloadUI.png");

	// スプライト生成
	sReloadUI_ = std::make_unique<Sprite>();
	sReloadUI_->SetAnchorPoint({ 0.5f, 0.5f });
	sReloadUI_->SetPosition({ winSize.x / 2.0f, winSize.y / 2.0f });
	sReloadUI_->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });
#pragma endregion
}

Player::~Player()
{
	CollisionManager::GetInstance()->RemoveCollider(eyeCollider_.get());
	CollisionManager::GetInstance()->RemoveCollider(playerCollider_.get());
}

void Player::Update()
{
	// 状態別更新処理
	(this->*stateTable[state_])();

	if (key_->TriggerKey(DIK_0)) {
		uint64_t s = Util::GetTime();
	}

	// プレイヤーのコライダー更新
	playerCollider_->SetOffset(camera_->GetEye() + float3(0.0f, 1.0f, 0.0f));

	OnCollision();

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
	// クロスヘア描画
	sCrossHair_->Draw(crossHairHandle_);

	// リロードUI描画
	if (isReload_) sReloadUI_->Draw(reloadUIHandle_);

	// 最大弾数を表示
	sMaxBulletUI_[0]->Draw(numberHandle_[maxBullet_ / 10]);
	sMaxBulletUI_[1]->Draw(numberHandle_[maxBullet_ % 10]);

	// 残弾数を表示
	sNowBulletUI_[0]->Draw(numberHandle_[nowBullet_ / 10]);
	sNowBulletUI_[1]->Draw(numberHandle_[nowBullet_ % 10]);

	// 残弾数表示枠を描画
	sBulletValueDisplayFrame_->Draw(bulletValueDisplayFrameHandle_);
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

	// リロード処理
	Reload();

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

	// リロード処理
	Reload();

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
	bullets_.emplace_back(std::make_unique<Bullet>(mBullet_.get(), BulletType::PLAYER, camera_->GetEye(), forwardVec_));
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
	// リロード開始時間[s]
	static uint64_t startReloadTime = 0;

	// リロードUI画像の角度
	static float rotaY = 0.0f;

	// [R]キーが押されたらリロードを開始
	if (key_->TriggerKey(DIK_R) && isReload_ == false) {
		isReload_ = true;
		startReloadTime = Util::GetTime();
		nowBullet_ = 0;
	}

	// リロードしていたらする処理
	if (isReload_) {

		// リロード画像を回転
		rotaY -= 3.0f;
		sReloadUI_->SetRotation(rotaY);

		// 何秒リロードしたか
		uint64_t elapsedReloadTime = Util::GetTime() - startReloadTime;
		// リロード時間を超えたらリロードを終える
		if (elapsedReloadTime >= reloadTime_) {
			isReload_ = false;
			nowBullet_ = maxBullet_;
		}
	}
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
	if (playerCollider_->GetIsHit()) {
		hp_ -= 1;
	}
}