#include "Player.h"
#include "CollisionAttribute.h"
#include "Util.h"
#include "WinAPI.h"
#include "Texture.h"

#include <imgui_impl_DX12.h>

Player::Player()
{
}

Player::~Player()
{
	colMgr_->RemoveCollider(playerCol_.get());
	colMgr_->RemoveCollider(legCol_.get());
	colMgr_->RemoveCollider(climbCol_.get());
	colMgr_->RemoveCollider(shotCol_.get());
}

void Player::Initialize()
{
	// ウィンドウサイズを取得
	Vector2 winSize = {
		static_cast<float>(WinAPI::GetInstance()->GetWidth()),
		static_cast<float>(WinAPI::GetInstance()->GetHeight()) };

#pragma region インスタンス
	key_ = Key::GetInstance();
	mouse_ = Mouse::GetInstance();
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -10.0f });
	Object3D::SetCamera(camera_.get());
	Sprite::SetCamera(camera_.get());
#pragma endregion

#pragma region モデル
	mSphere_ = std::make_unique<Model>("sphere");
	mBullet_ = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region オブジェクト
	oPlayer_ = std::make_unique<Object3D>(mSphere_.get());
#pragma endregion

#pragma region スプライト
	// クロスヘア
	sCrossHair_ = std::make_unique<Sprite>();
	sCrossHair_->SetAnchorPoint({ 0.5f, 0.5f });
	sCrossHair_->SetPosition({
		WinAPI::GetInstance()->GetWidth() / 2.0f,
		WinAPI::GetInstance()->GetHeight() / 2.0f });
	sCrossHair_->SetSize({ 26, 26 });

	// リロードUI
	sReloadUI_ = std::make_unique<Sprite>();
	sReloadUI_->SetAnchorPoint({ 0.5f, 0.5f });
	sReloadUI_->SetPosition({ winSize.x / 2.0f, winSize.y / 2.0f });
	sReloadUI_->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });

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
	sMaxBulletUI_[0]->SetPosition({ winSize.x - 85.75f, winSize.y - 30.0f });
	sMaxBulletUI_[1]->SetPosition({ winSize.x - 45.0f, winSize.y - 30.0f });

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

#pragma region 画像読み込み
	crossHairHandle_ = LoadTexture("Resources/crossHair.png");
	reloadUIHandle_ = LoadTexture("Resources/reloadUI.png");
	bulletValueDisplayFrameHandle_ = LoadTexture("Resources/BulletValueDisplayFrame.png");
	numberHandle_.resize(10);
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

	climbCol_ = std::make_unique<RayCollider>();
	climbCol_->SetAttribute(COL_CLIMB);
	climbCol_->SetObject3D(oPlayer_.get());
	colMgr_->AddCollider(climbCol_.get());

	// 弾を撃った時に使うコライダー
	shotCol_ = std::make_unique<RayCollider>();
	shotCol_->SetAttribute(COL_PLAYER_SHOT);
	shotCol_->SetObject3D(oPlayer_.get());
	colMgr_->AddCollider(shotCol_.get());
#pragma endregion
}

void Player::Update()
{
	// 状態別更新処理
	(this->*stateTable[state_])();

	for (auto it = bullets_.begin(); it != bullets_.end();) {
		// 弾の更新
		(*it)->Update();

		// 弾の生存フラグが[OFF]になったら弾を削除
		if ((*it)->GetIsAlive() == false) it = bullets_.erase(it);
		else ++it;
	}

	climbCol_->SetDir({ forwardVec_.x, 0.0f, forwardVec_.z });

	ImGui::Begin("Player");
	ImGui::Text("state = %s", stateName_[state_].c_str());
	ImGui::Text("distance = %f", legCol_->GetDistance());
	ImGui::End();
}

void Player::Draw3D()
{
	// 弾
	for (auto& it : bullets_) it->Draw();
}

void Player::DrawFront2D()
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

	if (climbCol_->GetIsHit() && climbCol_->GetDistance() <= 1.5f && key_->PushKey(DIK_W)) {
		state_ = CLIMB;
	}

#pragma region ダッシュする
	if (key_->PushKey(DIK_LSHIFT) && key_->PushKey(DIK_W)) {
		isDash_ = true;
	}

	else {
		isDash_ = false;
	}
#pragma endregion
}

void Player::MatUpdate()
{
	// カメラ
	camera_->Update();

	// オブジェクト
	oPlayer_->MatUpdate();

	// 弾
	for (auto& it : bullets_) it->MatUpdate();

	// クロスヘア
	sCrossHair_->MatUpdate();

	// 残弾数表示UIハンドル
	sBulletValueDisplayFrame_->MatUpdate();

	// 最大弾数表示スプライト
	sMaxBulletUI_[0]->MatUpdate();
	sMaxBulletUI_[1]->MatUpdate();

	// 残弾数表示スプライト
	sNowBulletUI_[0]->MatUpdate();
	sNowBulletUI_[1]->MatUpdate();

	// リロードUI
	sReloadUI_->MatUpdate();
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

	// 覗き込み
	//Ads();

	// 撃つ処理
	Shoot();

	// ジャンプ処理
	Jump();

	// リロード処理
	Reload();

	// 走行処理
	Dash();
}

void Player::Air()
{
	// 視点操作
	EyeMove();

	// 移動操作
	Move();

	// 覗き込み
	//Ads();

	// 撃つ処理
	Shoot();

	// 落下処理
	Fall();

	// リロード処理
	Reload();

	// 走行処理
	Dash();
}

void Player::Climb()
{
	gravity_ -= 0.2f;
	gravity_ = Util::Clamp(gravity_, 1.0f, -1.0f);
	camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
	oPlayer_->SetPosition(camera_->GetEye());

	// 視点操作
	EyeMove();

	// 移動操作
	Move();
}

void Player::Shoot()
{
	// フラグリセット
	shotCol_->SetAttribute(0);

	// 最後に弾を撃ってからの経過時間
	float result = (Util::GetTimrMSec() - shotTime_) / 1000.0f;

	// 残弾数が0以下ならこの後の処理を飛ばす
	if (nowBullet_ <= 0) return;

	// マウスを左クリックしていなかったらこの後の弾を飛ばす
	if (mouse_->PushMouseButton(MouseButton::M_LEFT) == false) return;

	// 経過時間が指定時間を過ぎていなかったら処理を飛ばす
	if (!(result >= shotInterval_)) return;

	// フラグを建てる
	shotCol_->SetAttribute(COL_PLAYER_SHOT);

	// 弾を撃った時間を記録
	shotTime_ = Util::GetTimrMSec();

	// 残弾を減らす
	nowBullet_--;

	Vector3 shotAngle = {
		eyeAngle_.x + Util::GetRandomFloat(-diffusivity_, diffusivity_),
		eyeAngle_.y + Util::GetRandomFloat(-diffusivity_, diffusivity_),
		0.0f
	};

	Vector3 shotVec = {
		sinf(Util::Degree2Radian(shotAngle.x)),
		cosf(Util::Degree2Radian(shotAngle.y)),
		cosf(Util::Degree2Radian(shotAngle.x))
	};

	shotCol_->SetDir(shotVec);

	// 弾を生成
	bullets_.emplace_front(std::make_unique<Bullet>(mBullet_.get(), BulletType::PLAYER, camera_->GetEye(), shotVec));
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
		startReloadTime = Util::GetTimeSec();
		nowBullet_ = 0;
	}

	// リロードしていたらする処理
	if (isReload_) {

		// リロード画像を回転
		rotaY -= 3.0f;
		sReloadUI_->SetRotation(rotaY);

		// 何秒リロードしたか
		uint64_t elapsedReloadTime = Util::GetTimeSec() - startReloadTime;
		// リロード時間を超えたらリロードを終える
		if (elapsedReloadTime >= reloadTime_) {
			isReload_ = false;
			nowBullet_ = maxBullet_;
		}
	}
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

void Player::Dash()
{
	static float time = 1.0f;

	if (isDash_) fovAngleY_ += 2.0f;
	else fovAngleY_ -= 2.0f;

	fovAngleY_ = Util::Clamp(fovAngleY_, 80.0f, 70.0f);

	camera_->SetFovAngleY(fovAngleY_);
}