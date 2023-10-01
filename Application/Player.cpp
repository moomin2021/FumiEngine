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
	// �E�B���h�E�T�C�Y���擾
	Vector2 winSize = {
		static_cast<float>(WinAPI::GetInstance()->GetWidth()),
		static_cast<float>(WinAPI::GetInstance()->GetHeight()) };

#pragma region �C���X�^���X
	key_ = Key::GetInstance();
	mouse_ = Mouse::GetInstance();
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -10.0f });
	Object3D::SetCamera(camera_.get());
	Sprite::SetCamera(camera_.get());
#pragma endregion

#pragma region ���f��
	mSphere_ = std::make_unique<Model>("sphere");
	mBullet_ = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region �I�u�W�F�N�g
	oPlayer_ = std::make_unique<Object3D>(mSphere_.get());
#pragma endregion

#pragma region �X�v���C�g
	// �N���X�w�A
	sCrossHair_ = std::make_unique<Sprite>();
	sCrossHair_->SetAnchorPoint({ 0.5f, 0.5f });
	sCrossHair_->SetPosition({
		WinAPI::GetInstance()->GetWidth() / 2.0f,
		WinAPI::GetInstance()->GetHeight() / 2.0f });
	sCrossHair_->SetSize({ 26, 26 });

	// �����[�hUI
	sReloadUI_ = std::make_unique<Sprite>();
	sReloadUI_->SetAnchorPoint({ 0.5f, 0.5f });
	sReloadUI_->SetPosition({ winSize.x / 2.0f, winSize.y / 2.0f });
	sReloadUI_->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });

	// �c�e���\��UI�p�̃X�v���C�g�𐶐�
	sBulletValueDisplayFrame_ = std::make_unique<Sprite>();
	sBulletValueDisplayFrame_->SetAnchorPoint({ 0.5f, 0.5f });
	sBulletValueDisplayFrame_->SetSize({ 300.0f, 140.0f });
	sBulletValueDisplayFrame_->SetPosition({ winSize.x - 150.0f, winSize.y - 70.0f });

	// �ő�e���\���p�X�v���C�g����
	sMaxBulletUI_.resize(2);
	sMaxBulletUI_[0] = std::make_unique<Sprite>();
	sMaxBulletUI_[1] = std::make_unique<Sprite>();
	sMaxBulletUI_[0]->SetAnchorPoint({ 1.0f, 1.0f });
	sMaxBulletUI_[1]->SetAnchorPoint({ 1.0f, 1.0f });
	sMaxBulletUI_[0]->SetSize({ 35.25f, 54.0f });
	sMaxBulletUI_[1]->SetSize({ 35.25f, 54.0f });
	sMaxBulletUI_[0]->SetPosition({ winSize.x - 85.75f, winSize.y - 30.0f });
	sMaxBulletUI_[1]->SetPosition({ winSize.x - 45.0f, winSize.y - 30.0f });

	// �c�e���\���X�v���C�g
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

#pragma region �摜�ǂݍ���
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

#pragma region �R���C�_�[
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

	// �e�����������Ɏg���R���C�_�[
	shotCol_ = std::make_unique<RayCollider>();
	shotCol_->SetAttribute(COL_PLAYER_SHOT);
	shotCol_->SetObject3D(oPlayer_.get());
	colMgr_->AddCollider(shotCol_.get());
#pragma endregion
}

void Player::Update()
{
	// ��ԕʍX�V����
	(this->*stateTable[state_])();

	for (auto it = bullets_.begin(); it != bullets_.end();) {
		// �e�̍X�V
		(*it)->Update();

		// �e�̐����t���O��[OFF]�ɂȂ�����e���폜
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
	// �e
	for (auto& it : bullets_) it->Draw();
}

void Player::DrawFront2D()
{
	// �N���X�w�A�`��
	sCrossHair_->Draw(crossHairHandle_);

	// �����[�hUI�`��
	if (isReload_) sReloadUI_->Draw(reloadUIHandle_);

	// �ő�e����\��
	sMaxBulletUI_[0]->Draw(numberHandle_[maxBullet_ / 10]);
	sMaxBulletUI_[1]->Draw(numberHandle_[maxBullet_ % 10]);

	// �c�e����\��
	sNowBulletUI_[0]->Draw(numberHandle_[nowBullet_ / 10]);
	sNowBulletUI_[1]->Draw(numberHandle_[nowBullet_ % 10]);

	// �c�e���\���g��`��
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

#pragma region �_�b�V������
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
	// �J����
	camera_->Update();

	// �I�u�W�F�N�g
	oPlayer_->MatUpdate();

	// �e
	for (auto& it : bullets_) it->MatUpdate();

	// �N���X�w�A
	sCrossHair_->MatUpdate();

	// �c�e���\��UI�n���h��
	sBulletValueDisplayFrame_->MatUpdate();

	// �ő�e���\���X�v���C�g
	sMaxBulletUI_[0]->MatUpdate();
	sMaxBulletUI_[1]->MatUpdate();

	// �c�e���\���X�v���C�g
	sNowBulletUI_[0]->MatUpdate();
	sNowBulletUI_[1]->MatUpdate();

	// �����[�hUI
	sReloadUI_->MatUpdate();
}

void (Player::* Player::stateTable[]) () = {
	&Player::Normal,// �ʏ���
	&Player::Air,	// �󒆏��
	&Player::Climb,	// �o����
};

void Player::Normal()
{
	// ���_����
	EyeMove();

	// �ړ�����
	Move();

	// �`������
	//Ads();

	// ������
	Shoot();

	// �W�����v����
	Jump();

	// �����[�h����
	Reload();

	// ���s����
	Dash();
}

void Player::Air()
{
	// ���_����
	EyeMove();

	// �ړ�����
	Move();

	// �`������
	//Ads();

	// ������
	Shoot();

	// ��������
	Fall();

	// �����[�h����
	Reload();

	// ���s����
	Dash();
}

void Player::Climb()
{
	gravity_ -= 0.2f;
	gravity_ = Util::Clamp(gravity_, 1.0f, -1.0f);
	camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
	oPlayer_->SetPosition(camera_->GetEye());

	// ���_����
	EyeMove();

	// �ړ�����
	Move();
}

void Player::Shoot()
{
	// �t���O���Z�b�g
	shotCol_->SetAttribute(0);

	// �Ō�ɒe�������Ă���̌o�ߎ���
	float result = (Util::GetTimrMSec() - shotTime_) / 1000.0f;

	// �c�e����0�ȉ��Ȃ炱�̌�̏������΂�
	if (nowBullet_ <= 0) return;

	// �}�E�X�����N���b�N���Ă��Ȃ������炱�̌�̒e���΂�
	if (mouse_->PushMouseButton(MouseButton::M_LEFT) == false) return;

	// �o�ߎ��Ԃ��w�莞�Ԃ��߂��Ă��Ȃ������珈�����΂�
	if (!(result >= shotInterval_)) return;

	// �t���O�����Ă�
	shotCol_->SetAttribute(COL_PLAYER_SHOT);

	// �e�����������Ԃ��L�^
	shotTime_ = Util::GetTimrMSec();

	// �c�e�����炷
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

	// �e�𐶐�
	bullets_.emplace_front(std::make_unique<Bullet>(mBullet_.get(), BulletType::PLAYER, camera_->GetEye(), shotVec));
}

void Player::Reload()
{
	// �����[�h�J�n����[s]
	static uint64_t startReloadTime = 0;

	// �����[�hUI�摜�̊p�x
	static float rotaY = 0.0f;

	// [R]�L�[�������ꂽ�烊���[�h���J�n
	if (key_->TriggerKey(DIK_R) && isReload_ == false) {
		isReload_ = true;
		startReloadTime = Util::GetTimeSec();
		nowBullet_ = 0;
	}

	// �����[�h���Ă����炷�鏈��
	if (isReload_) {

		// �����[�h�摜����]
		rotaY -= 3.0f;
		sReloadUI_->SetRotation(rotaY);

		// ���b�����[�h������
		uint64_t elapsedReloadTime = Util::GetTimeSec() - startReloadTime;
		// �����[�h���Ԃ𒴂����烊���[�h���I����
		if (elapsedReloadTime >= reloadTime_) {
			isReload_ = false;
			nowBullet_ = maxBullet_;
		}
	}
}

void Player::Move()
{
	// ���͕���
	Vector3 inputVec = {
		static_cast<float>(key_->PushKey(DIK_D) - key_->PushKey(DIK_A)),
		0.0f,
		static_cast<float>(key_->PushKey(DIK_W) - key_->PushKey(DIK_S))
	};

	// �ړ����͂�����Ă��������������
	if (inputVec.length() > 0.0f)	moveSpd_ += moveAcc_;
	else							moveSpd_ -= moveAcc_;

	// ���x����
	if (isDash_) moveSpd_ = Util::Clamp(moveSpd_, dashSpd_, 0.0f);
	else moveSpd_ = Util::Clamp(moveSpd_, maxMoveSpd_, 0.0f);

	// ���ʃx�N�g���̈ړ���
	Vector3 forwardMoveVec = {
		inputVec.z * forwardVec_.x,
		0.0f,
		inputVec.z * forwardVec_.z
	};

	// �E�����x�N�g���̈ړ���
	Vector3 rightMoveVec = {
		inputVec.x * rightVec_.x,
		0.0f,
		inputVec.x * rightVec_.z
	};

	// �ړ��x�N�g���̌���
	Vector3 resultVec = forwardMoveVec + rightMoveVec;
	resultVec.normalize();

	// �J�������X�V
	camera_->SetEye(camera_->GetEye() + resultVec * moveSpd_);
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);

	// �I�u�W�F�N�g�̈ʒu���X�V
	oPlayer_->SetPosition(camera_->GetEye());
}

void Player::EyeMove()
{
	// �}�E�X�̈ړ��ʕ����_�̊p�x�ɉ��Z
	eyeAngle_.x += mouse_->GetMouseVelosity().x * sencivity_;
	eyeAngle_.y += mouse_->GetMouseVelosity().y * sencivity_;

	// ���_�ړ��̏㉺�ɐ�����t����
	eyeAngle_.y = Util::Clamp(eyeAngle_.y, 120.0f, 60.0f);

	// �O���x�N�g���v�Z
	forwardVec_ = {
		sinf(Util::Degree2Radian(eyeAngle_.x)),
		cosf(Util::Degree2Radian(eyeAngle_.y)),
		cosf(Util::Degree2Radian(eyeAngle_.x))
	};

	// �O���x�N�g�����K��
	forwardVec_.normalize();

	// �E�x�N�g���v�Z
	rightVec_ = Vector3(forwardVec_.x, 0.0f, forwardVec_.z);
	rightVec_ = -rightVec_.cross(rightVec_ + Vector3(0.0f, 1.0f, 0.0f));

	// �E�x�N�g���v�Z
	rightVec_.normalize();

	// �J�����ݒ�
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);

	// �I�u�W�F�N�g�̈ʒu���X�V
	oPlayer_->SetPosition(camera_->GetEye());
}

void Player::Jump()
{
	// [SPACE]�������ꂽ�������ɉ���������
	if (key_->TriggerKey(DIK_SPACE)) {
		// �d�͂��X�V
		gravity_ = -jumpSpd_;

		// �J�����̈ʒu���X�V
		camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
		camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);

		// �Փ˔���p�̃R���C�_�[���X�V
		oPlayer_->SetPosition(camera_->GetEye());
	}
}

void Player::Fall()
{
	// �d�͉����x�����Z
	gravity_ += gAcc_;// �d�͉����x�����Z

	// �ő�d�͂𒴂��Ȃ��悤��
	gravity_ = Util::Clamp(gravity_, maxGravity_, -1000.0f);

	// �J�������X�V
	camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);

	// �Փ˔���p�̃R���C�_�[���X�V
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