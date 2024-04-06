#include "Player.h"

#include "Easing.h"
#include "Revolver.h"

#include "CollisionAttribute.h"

void Player::Initialize(const Vector3& inPos)
{
	// シングルトンインスタンス取得
	key_ = Key::GetInstance();
	mouse_ = Mouse::GetInstance();
	colMgr_ = CollisionManager3D::GetInstance();

	// オブジェクト生成
	model_ = std::make_unique<Model>("sphere");
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition(inPos);

	// 衝突判定
	playerColData_.offset = { 0.0f, 1.0f, 0.0f };
	playerColData_.radius = { 0.25f, 1.0f, 0.25f };
	playerC_ = std::make_unique<AABBCollider>();
	playerC_->SetOffset(playerColData_.offset);
	playerC_->SetRadius(playerColData_.radius);
	playerC_->SetAttribute(COL_PLAYER);
	playerC_->SetObject3D(object_.get());
	colMgr_->AddCollider(playerC_.get());

	groundColData_.offset = { 0.0f, 1.75f, 0.0f };
	groundColData_.rayDir = { 0.0f, -1.0f, 0.0f };
	groundC_ = std::make_unique<RayCollider>();
	groundC_->SetDir(groundColData_.rayDir);
	groundC_->SetOffSet(groundColData_.offset);
	groundC_->SetPushBackDistance(groundColData_.offset.y);
	groundC_->SetAttribute(COL_LEG);
	groundC_->SetObject3D(object_.get());
	colMgr_->AddPushBackRayCollider(groundC_.get());

	shotColData_.offset = { 0.0f, 1.75f, 0.0f };
	shotC_ = std::make_unique<RayCollider>();
	shotC_->SetOffSet(shotColData_.offset);
	shotC_->SetObject3D(object_.get());
	colMgr_->AddCollider(shotC_.get());

	// カメラ生成
	cameraOffset_ = { 0.0f, 1.75f, 0.0f };
	camera_ = std::make_unique<Camera>();
	camera_->SetEye(object_->GetPosition() + cameraOffset_);

	// 武器生成
	weapon_ = std::make_unique<Revolver>(object_.get());
	weapon_->Initialize();
}

void Player::Update()
{
	EyeMove();
	Move();
	Dash();
	Shot();

	if (key_->PushKey(DIK_LSHIFT)) isDash_ = true;
	else isDash_ = false;
	if (key_->TriggerKey(DIK_R)) weapon_->StartReload();
	weapon_->SetADS(mouse_->PushMouseButton(MouseButton::M_RIGHT));
	weapon_->Update();
	weapon_->Pitching(isMove_);

	FovAngleUpdate();
	JudgmentDeath();
}

void Player::Draw()
{
	weapon_->Draw();
}

void Player::Collision()
{
	weapon_->Collision();
	weapon_->PosRotaUpdate(forwardVec_, rightVec_, eyeAngle_);
	CameraUpdate();
}

void Player::MatUpdate()
{
	camera_->Update();
	object_->MatUpdate();
	weapon_->MatUpdate();
}

void Player::Finalize()
{
	weapon_->Finalize();
	colMgr_->RemoveCollider(playerC_.get());
	colMgr_->RemovePushBackRayCollider(groundC_.get());
	colMgr_->RemoveCollider(shotC_.get());
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
	forwardVec_.normalize();

	// 右ベクトル計算
	rightVec_ = Vector3(forwardVec_.x, 0.0f, forwardVec_.z);
	rightVec_ = -rightVec_.cross(rightVec_ + Vector3(0.0f, 1.0f, 0.0f));
	rightVec_.normalize();

	// カメラの向き更新
	camera_->SetTarget(camera_->GetEye() + forwardVec_);
}

void Player::Move()
{
	// 入力方向
	Vector3 inputVec = {
		static_cast<float>(key_->PushKey(DIK_D) - key_->PushKey(DIK_A)),
		0.0f,
		static_cast<float>(key_->PushKey(DIK_W) - key_->PushKey(DIK_S))
	};

	if (inputVec.length()) isMove_ = true;
	else isMove_ = false;

	// 移動入力がされていたら加速させる
	if (inputVec.length() > 0.0f)	moveSpd_ += moveAcc_;
	else							moveSpd_ -= moveAcc_;

	// 速度制限
	if (isDash_) moveSpd_ = Util::Clamp(moveSpd_, maxDashSpd_, 0.0f);
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
	Vector3 result = object_->GetPosition() + moveVec * moveSpd_;

	// オブジェクトの位置を更新
	object_->SetPosition(result);
}

void Player::Dash()
{
	if (weapon_->GetIsADS()) return;

	if (isDash_) nowFovAngle_ += fovAngleIncrDecValue_;
	else nowFovAngle_ -= fovAngleIncrDecValue_;

	nowFovAngle_ = Util::Clamp(nowFovAngle_, maxFovAngle_, minFovAngle_);

	camera_->SetFovAngleY(nowFovAngle_);
}

void Player::Shot()
{
	shotC_->SetAttribute(0);

	// 残弾数が0以下ならこの後の処理を飛ばす
	if (weapon_->GetNowBullet() <= 0) return;

	// マウスを左クリックしていなかったらこの後の弾を飛ばす
	if (mouse_->PushMouseButton(MouseButton::M_LEFT) == false) return;

	// 経過時間が指定時間を過ぎていなかったら処理を飛ばす
	if (weapon_->Shot() == false) return;

	// フラグを建てる
	shotC_->SetAttribute(COL_PLAYER_SHOT);

	// 拡散の計算
	nowDiffusivity_ = Easing::lerp(maxDiffusivity_, 0.0f, weapon_->GetADSRate());
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

void Player::FovAngleUpdate()
{
	nowFovAngle_ = Easing::lerp(defFovAngle_, minFovAngle_, weapon_->GetADSRate());
	camera_->SetFovAngleY(nowFovAngle_);
}

void Player::JudgmentDeath()
{
	// HPが0以下なら死亡判定
	if (nowHP_ > 0) return;
	isAlive_ = false;
}

void Player::CameraUpdate()
{
	// カメラの座標とターゲットの位置を計算し更新する
	Vector3 pos = object_->GetPosition() + cameraOffset_;
	Vector3 target = pos + forwardVec_;
	camera_->SetEye(pos);
	camera_->SetTarget(target);
}