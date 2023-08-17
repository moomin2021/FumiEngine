#include "Player.h"
#include "CollisionAttribute.h"

#include <imgui_impl_DX12.h>

Player::~Player()
{
	// 削除
	colMgr_->RemoveCollider(playerCol_.get());
}

void Player::Initialize()
{
#pragma region インスタンス取得
	key_	= Key::GetInstance();	// キーボード
	mouse_	= Mouse::GetInstance();	// マウス
	colMgr_ = CollisionManager::GetInstance();// コリジョンマネージャー
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 2.0f, 0.0f });
#pragma endregion

#pragma region モデル
	model_ = std::make_unique<Model>("sphere");
	mBullet_ = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region オブジェクト
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition({ 0.0f, 2.0f, 0.0f });
#pragma endregion

#pragma region コライダー
	playerCol_ = std::make_unique<SphereCollider>();
	playerCol_->SetAttribute(COL_PLAYER);
	playerCol_->LinkObject3D(object_.get());
	colMgr_->AddCollider(playerCol_.get());// 追加
#pragma endregion

	// カメラをオブジェクト3Dに適用
	Object3D::SetCamera(camera_.get());
}

void Player::Update()
{
	// 状態別更新処理
	(this->*stateTable[state_])();

	float3 pos = camera_->GetEye();

	ImGui::Begin("Player");
	ImGui::Text("Pos = {%f, %f, %f}", pos.x, pos.y, pos.z);
	ImGui::Text("ForwardVec = {%f, %f, %f}", forwardVec_.x, forwardVec_.y, forwardVec_.z);
}

void Player::Draw()
{
	//object_->Draw();

	for (auto& it : bullets_) it->Draw();
}

void Player::ObjUpdate()
{
	object_->Update();

	camera_->Update();
}

void Player::OnCollision()
{
	ImGui::Text("IsHit = %d", playerCol_->GetIsHit());
	ImGui::End();

	if (playerCol_->GetIsHit()) {
		float3 reject = playerCol_->GetReject();
		camera_->SetEye(camera_->GetEye() + reject);
		camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
		object_->SetPosition(camera_->GetEye());
	}
}

void (Player::* Player::stateTable[]) () = {
	&Player::Normal,// 通常状態
	&Player::Air,	// 空中状態
};

void Player::Normal()
{
	// 視点操作
	EyeMove();

	// 移動操作
	Move();

	// 撃つ処理
	Shoot();
}

void Player::Air()
{
}

void Player::Shoot()
{
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		// 弾の更新
		(*it)->Update();

		// 弾の生存フラグが[OFF]になったら弾を削除
		if ((*it)->GetIsAlive() == false) it = bullets_.erase(it);
		else ++it;
	}

	// 最後に弾を撃ってからの経過時間
	float result = (Util::GetTimrMil() - shotTime_) / 1000.0f;

	// 残弾数が0以下ならこの後の処理を飛ばす
	if (nowBullet_ <= 0) return;

	// マウスを左クリックしていなかったらこの後の弾を飛ばす
	if (mouse_->PushMouseButton(MouseButton::M_LEFT) == false) return;

	// 経過時間が指定時間を過ぎていなかったら処理を飛ばす
	if (!(result >= shotInterval_)) return;

	// 弾を撃った時間を記録
	shotTime_ = Util::GetTimrMil();

	// 残弾を減らす
	nowBullet_--;

	// 弾を生成
	bullets_.emplace_back(std::make_unique<Bullet>(mBullet_.get(), BulletType::PLAYER, camera_->GetEye(), forwardVec_));
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
	moveSpd_ = Util::Clamp(moveSpd_, maxMoveSpd_, 0.0f);

	// 正面ベクトルの移動量
	Vector3 forwardMoveVec = {
		inputVec.z* forwardVec_.x,
		0.0f,
		inputVec.z* forwardVec_.z
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

	// オブジェクトの更新
	object_->SetPosition(camera_->GetEye());
}

void Player::EyeMove()
{
	// 視点角度
	static float3 eyeAngle = { 0.0f, 90.0f, 0.0f };

	// マウスの移動量分視点の角度に加算
	eyeAngle.x += mouse_->GetMouseVelosity().x * sencivity_;
	eyeAngle.y += mouse_->GetMouseVelosity().y * sencivity_;

	// 視点移動の上下に制限を付ける
	eyeAngle.y = Util::Clamp(eyeAngle.y, 180.0f, 0.0f);

	// 前方ベクトル計算
	forwardVec_ = {
		sinf(Util::Degree2Radian(eyeAngle.x)),
		cosf(Util::Degree2Radian(eyeAngle.y)),
		cosf(Util::Degree2Radian(eyeAngle.x))
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