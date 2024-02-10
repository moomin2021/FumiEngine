#include "Player.h"
#include "WinAPI.h"
#include "Sprite.h"
#include "Line3D.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Instancing3D.h"
#include "ParticleEmitter.h"

#include "CollisionAttribute.h"

#include <imgui_impl_DX12.h>

Player::~Player()
{
	colMgr_->RemoveCollider(playerC_.get());
	colMgr_->RemoveCollider(groundJudgmentC_.get());
	colMgr_->RemoveCollider(shotC_.get());
}

void Player::Initialize(const Vector3& startPos)
{
#pragma region インスタンス
	key_ = Key::GetInstance();
	mouse_ = Mouse::GetInstance();
	colMgr_ = CollisionManager3D::GetInstance();
#pragma endregion

#pragma region モデル
	playerM_ = std::make_unique<Model>("sphere");
	sheriffM_ = std::make_unique<Model>("Sheriff");
#pragma endregion

#pragma region オブジェクト
	playerO_ = std::make_unique<Object3D>(playerM_.get());
	playerO_->SetPosition(startPos);

	sheriffO_ = std::make_unique<Object3D>(sheriffM_.get());
	sheriffO_->SetScale({ 0.1f, 0.1f, 0.1f });
#pragma endregion

#pragma region コライダー
	playerC_ = std::make_unique<AABBCollider>();
	playerC_->SetRadius({ 0.3f, 0.8f, 0.3f });
	playerC_->SetOffset({ 0.0f, 0.9f, 0.0f });
	playerC_->SetAttribute(COL_PLAYER);
	playerC_->SetObject3D(playerO_.get());
	colMgr_->AddCollider(playerC_.get());

	groundJudgmentC_ = std::make_unique<RayCollider>();
	groundJudgmentC_->SetDir({ 0.0f, -1.0f, 0.0f });// レイを真下に向ける
	groundJudgmentC_->SetOffSet({ 0.0f, cameraHeight_, 0.0f });
	groundJudgmentC_->SetAttribute(COL_LEG);
	groundJudgmentC_->SetObject3D(playerO_.get());
	colMgr_->AddCollider(groundJudgmentC_.get());

	shotC_ = std::make_unique<RayCollider>();
	shotC_->SetOffSet({ 0.0f, cameraHeight_, 0.0f });
	shotC_->SetObject3D(playerO_.get());
	colMgr_->AddCollider(shotC_.get());
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
#pragma endregion
}

void Player::Update()
{
	// 状態別更新処理
	(this->*state[state_])();

	// リコイル処理
	Recoil();

	damageCount_++;
	damageCount_ = (uint8_t)Util::Clamp((uint8_t)damageCount_, 100, 0);
	Knock();
}

void Player::Draw()
{
	sheriffO_->Draw();
}

void Player::OnCollision()
{
	// 接地判定
	GroundingJudgment();

	// ダッシュ入力をとる
	if (key_->PushKey(DIK_LSHIFT)) isDash_ = true;
	else isDash_ = false;

	// ADS入力をとる
	if (mouse_->PushMouseButton(MouseButton::M_RIGHT)) isAds_ = true;
	else isAds_ = false;
}

void Player::MatUpdate()
{
	// カメラの位置を更新
	camera_->SetEye(playerO_->GetPosition() + Vector3{ 0.0f, cameraHeight_, 0.0f });
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);

	// ピストルオブジェクトの座標計算
	Sheriff();

	camera_->Update();
	playerO_->MatUpdate();
	sheriffO_->MatUpdate();
}

void Player::Debug()
{
	Vector3 playerPos = playerO_->GetPosition();
	ImGui::Begin("Player");
	ImGui::Text("state = %s", stateName_[state_].c_str());
	ImGui::Text("playerPos = { %f, %f, %f }", playerPos.x, playerPos.y, playerPos.z);
	ImGui::Text("distance = %f", groundJudgmentC_->GetDistance());
	ImGui::End();

	eyeAngle_.x = 0.0f;
}

void Player::SetKnock(const Vector3& vec)
{
	if (damageCount_ >= damageCooldown_)
	{
		knockVec_ = vec + Vector3(0.0f, 1.0f, 0.0f);
		knockSpd_ = 0.5f;
		damageCount_ = 0;
		hp_ -= 1;
		camera_->SetUp(hitAngle_);
	}
}

void Player::CheckSceneChange()
{
	if (hp_ > 0) return;
	SceneManager::GetInstance()->SceneTransition(SCENE::GAMEOVER);
	WinAPI::GetInstance()->DisplayCursor(true);
	WinAPI::GetInstance()->SetClipCursor(false);
}

void (Player::* Player::state[]) () = {
	&Player::Normal,	// 通常状態
	&Player::Air,		// 空中状態
};

void Player::Normal()
{
	// 視点操作
	EyeMove();

	// 移動操作
	Move();

	// 覗き込み
	Ads();

	// 撃つ処理
	Shot();

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
	Ads();

	// 撃つ処理
	Shot();

	// ジャンプ処理
	//Jump();

	// 落下処理
	Fall();

	// リロード処理
	Reload();

	// 走行処理
	Dash();
}

void Player::EyeMove()
{
	// マウスの移動量分視点の角度に加算
	eyeAngle_.x += mouse_->GetMouseVelosity().x * sencivity_;
	eyeAngle_.y += mouse_->GetMouseVelosity().y * sencivity_;

	// 視点移動の上下に制限を付ける
	eyeAngle_.y = Util::Clamp(eyeAngle_.y, 180.0f, 0.0f);

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
	Vector3 moveVec = forwardMoveVec + rightMoveVec;
	moveVec.normalize();

	// 最終的な座標
	Vector3 result = playerO_->GetPosition() + moveVec * moveSpd_;

	// オブジェクトの位置を更新
	playerO_->SetPosition(result);
}

void Player::Jump()
{
	// [SPACE]が押されたら上方向に加速させる
	if (key_->TriggerKey(DIK_SPACE))
	{
		// 重力を更新
		gravity_ = -jumpSpd_;

		Fall();
	}
}

void Player::Fall()
{
	// 重力加速度を加算
	gravity_ += gAcc_;// 重力加速度を加算

	// 最大重力を超えないように
	gravity_ = Util::Clamp(gravity_, maxGravity_, -1000.0f);

	// 最終的な座標
	Vector3 result = playerO_->GetPosition() + Vector3{ 0.0f, -1.0f, 0.0f } *gravity_;

	// 衝突判定用のコライダーを更新
	playerO_->SetPosition(result);
}

void Player::Dash()
{
	if (isAds_) return;

	// ダッシュ入力されたら視野角を広くする
	if (isDash_) nowFovAngle_ += fovAngleIncrDecValue_;

	// されなかったら視野角を狭くする
	else nowFovAngle_ -= fovAngleIncrDecValue_;

	// クランプ
	nowFovAngle_ = Util::Clamp(nowFovAngle_, maxFovAngle_, minFovAngle_);

	// カメラの視野角を設定
	camera_->SetFovAngleY(nowFovAngle_);
}

void Player::Ads()
{
	// 右クリック入力されたらADS率を増やす
	if (isAds_) adsRate_ += adsRateIncrDecValue_;

	// されていなかったら減らす
	else adsRate_ -= adsRateIncrDecValue_;

	// ADS率のクランプ
	adsRate_ = Util::Clamp(adsRate_, 1.0f, 0.0f);

	// 視野角の調整
	nowFovAngle_ = Easing::lerp(defFovAngle_, minFovAngle_, adsRate_);
	nowFovAngle_ = Util::Clamp(nowFovAngle_, defFovAngle_, minFovAngle_);

	// 視野角の設定
	camera_->SetFovAngleY(nowFovAngle_);
}

void Player::Shot()
{
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) return;
	if (key_->PushKey(DIK_Q)) return;

	// フラグリセット
	shotC_->SetAttribute(0);

	// 最後に弾を撃ってからの経過時間
	float result = (Util::GetTimrMSec() - shotTime_) / 1000.0f;

	// 残弾数が0以下ならこの後の処理を飛ばす
	if (nowBullet_ <= 0) return;

	// マウスを左クリックしていなかったらこの後の弾を飛ばす
	if (mouse_->PushMouseButton(MouseButton::M_LEFT) == false) return;

	// 経過時間が指定時間を過ぎていなかったら処理を飛ばす
	if (!(result >= shotInterval_)) return;

	// フラグを建てる
	shotC_->SetAttribute(COL_PLAYER_SHOT);

	// 弾を撃った時間を記録
	shotTime_ = Util::GetTimrMSec();

	// 残弾を減らす
	nowBullet_--;

	// 拡散の計算
	nowDiffusivity_ = Easing::lerp(maxDiffusivity_, 0.0f, adsRate_);
	Vector3 shotAngle = {
		eyeAngle_.x + Util::GetRandomFloat(-nowDiffusivity_, nowDiffusivity_),
		eyeAngle_.y + Util::GetRandomFloat(-nowDiffusivity_, nowDiffusivity_),
		0.0f
	};

	Vector3 shotVec = {
		sinf(Util::Degree2Radian(eyeAngle_.x)),
		cosf(Util::Degree2Radian(eyeAngle_.y)),
		cosf(Util::Degree2Radian(eyeAngle_.x))
	};

	shotVec.normalize();

	shotC_->SetDir(shotVec);

	// 反動をつける
	nowRecoilEyeAngle_ = maxRecoilEyeAngle_;
}

void Player::Reload()
{
	// リロード開始時間[s]
	static uint64_t startReloadTime = 0;

	// [R]キーが押されたらリロードを開始
	if (key_->TriggerKey(DIK_R) && isReload_ == false)
	{
		isReload_ = true;
		startReloadTime = Util::GetTimeSec();
		nowBullet_ = 0;
	}

	// リロードしていたらする処理
	if (isReload_)
	{
		// 何秒リロードしたか
		uint64_t elapsedReloadTime = Util::GetTimeSec() - startReloadTime;
		// リロード時間を超えたらリロードを終える
		if (elapsedReloadTime >= reloadTime_)
		{
			isReload_ = false;
			nowBullet_ = maxBullet_;
		}
	}
}

void Player::Knock()
{
	float rate = (float)damageCount_ / (damageCooldown_ - 20);
	rate = Util::Clamp(rate, 1.0f, 0.0f);
	up_.x = Easing::Quint::easeIn(hitAngle_.x, 0.0f, rate);
	knockSpd_ -= decKnockSpd_;
	knockSpd_ = Util::Clamp(knockSpd_, 10.0f, 0.0f);
	playerO_->SetPosition(playerO_->GetPosition() + knockVec_ * knockSpd_);
	camera_->SetEye(playerO_->GetPosition());
	camera_->SetTarget(camera_->GetEye() + forwardVec_);
	camera_->SetUp(up_);
}

void Player::Recoil()
{
	// リコイル角度の減少
	nowRecoilEyeAngle_ -= decRecoilEyeAngle_;

	// クランプ
	nowRecoilEyeAngle_ = Util::Clamp(nowRecoilEyeAngle_, maxRecoilEyeAngle_, 0.0f);

	// カメラのターゲットにリコイル角度を適用
	Vector3 recoilVec_ = {
		sinf(Util::Degree2Radian(eyeAngle_.x)),
		cosf(Util::Degree2Radian(eyeAngle_.y - nowRecoilEyeAngle_)),
		cosf(Util::Degree2Radian(eyeAngle_.x))
	};

	// カメラ設定
	camera_->SetTarget(camera_->GetEye() + (recoilVec_));
}

void Player::Sheriff()
{
	// ADS時の座標
	Vector3 adsPos = camera_->GetEye() + Vector3{ 0.0f, -0.1f, 0.0f } + (forwardVec_ * 0.5f);

	// 非ADS時の座標
	Vector3 noAdsPos = camera_->GetEye() + Vector3{ 0.0f, -0.2f, 0.0f } + (forwardVec_ * 0.5f) + (rightVec_ * 0.5f);

	// 最終的な座標
	Vector3 resultPos = { 0.0f, 0.0f, 0.0f };

	resultPos.x = Easing::lerp(noAdsPos.x, adsPos.x, adsRate_);
	resultPos.y = Easing::lerp(noAdsPos.y, adsPos.y, adsRate_);
	resultPos.z = Easing::lerp(noAdsPos.z, adsPos.z, adsRate_);

	sheriffO_->SetPosition(resultPos);
	sheriffO_->SetRotation({ (eyeAngle_.y + 90.0f), eyeAngle_.x, 180.0f });
	sheriffO_->MatUpdate();

	// リコイルの計算
	Vector3 recoilVec_ = {
		sinf(Util::Degree2Radian(eyeAngle_.x)),
		cosf(Util::Degree2Radian(eyeAngle_.y - nowRecoilEyeAngle_)),
		cosf(Util::Degree2Radian(eyeAngle_.x))
	};

	// カメラ設定
	camera_->SetTarget(camera_->GetEye() + (recoilVec_) * 10.0f);
}

void Player::GroundingJudgment()
{
	// 状態の設定
	state_ = State::AIR;

	// コライダーが衝突していなかったら処理を飛ばす
	if (!groundJudgmentC_->GetIsHit()) return;

	// 衝突したコライダーとの距離がカメラの高さより長かったら処理を飛ばす
	if (Util::Round(groundJudgmentC_->GetDistance(), 2) > cameraHeight_) return;

	// 押し戻す距離を計算
	Vector3 pushBackVec = (cameraHeight_ - groundJudgmentC_->GetDistance()) * Vector3{ 0.0f, 1.0f, 0.0f };

	// 押し戻した後の座標を計算
	Vector3 result = playerO_->GetPosition() + pushBackVec;

	// 設定
	playerO_->SetPosition(result);

	// 状態の設定
	state_ = State::NORMAL;

	// 重力を初期化
	gravity_ = 0.0f;
}