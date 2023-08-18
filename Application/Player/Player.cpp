#include "Player.h"
#include "CollisionAttribute.h"
#include "Texture.h"
#include "WinAPI.h"

#include <imgui_impl_DX12.h>

Player::~Player()
{
	// �폜
	colMgr_->RemoveCollider(playerCol_.get());
	colMgr_->RemoveCollider(legCol_.get());
	colMgr_->RemoveCollider(climbCol_.get());
	colMgr_->RemoveCollider(eyeCol_.get());
}

void Player::Initialize(EnemyManager* enemyMgr)
{
	// �E�B���h�E�T�C�Y���擾
	float2 winSize = {
		static_cast<float>(WinAPI::GetInstance()->GetWidth()),
		static_cast<float>(WinAPI::GetInstance()->GetHeight()) };

#pragma region �C���X�^���X�擾
	key_	= Key::GetInstance();	// �L�[�{�[�h
	mouse_	= Mouse::GetInstance();	// �}�E�X
	colMgr_ = CollisionManager::GetInstance();// �R���W�����}�l�[�W���[
	enemyMgr_ = enemyMgr;
#pragma endregion

#pragma region �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 2.0f, 0.0f });
	camera_->SetFovAngleY(fovAngleY_);
#pragma endregion

#pragma region ���f��
	model_ = std::make_unique<Model>("sphere");
	mBullet_ = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region �I�u�W�F�N�g
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition({ 0.0f, 2.0f, 0.0f });

	testObj_ = std::make_unique<Object3D>(model_.get());
	testObj_->SetScale({ 0.25f, 0.25f, 0.25f });
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
	// �v���C���[�̃R���C�_�[(�G����̍U����ǂƂ̏Փˏ����Ɏg�p)
	playerCol_ = std::make_unique<SphereCollider>();
	playerCol_->SetAttribute(COL_PLAYER);
	playerCol_->LinkObject3D(object_.get());
	colMgr_->AddCollider(playerCol_.get());// �o�^

	// �����̃R���C�_�[(���������Ɏg�p)
	legCol_ = std::make_unique<SphereCollider>(float3{0.0f, -1.0f, 0.0f});
	legCol_->SetAttribute(COL_LEG);
	legCol_->LinkObject3D(object_.get());
	colMgr_->AddCollider(legCol_.get());// �o�^

	// �Ǔo��Ɏg���R���C�_�[
	climbCol_ = std::make_unique<SphereCollider>(float3{0.0f, 0.0f, 0.0f}, 0.25f);
	climbCol_->SetAttribute(COL_FRONT);
	climbCol_->LinkObject3D(testObj_.get());
	colMgr_->AddCollider(climbCol_.get());

	// ���_�R���C�_�[
	eyeCol_ = std::make_unique<RayCollider>();
	eyeCol_->SetAttribute(COL_PLAYER_RAY);
	eyeCol_->LinkObject3D(object_.get());
	colMgr_->AddCollider(eyeCol_.get());

#pragma endregion

	// �J�������I�u�W�F�N�g3D�ɓK�p
	Object3D::SetCamera(camera_.get());
}

void Player::Update()
{
	// ��ԕʍX�V����
	(this->*stateTable[state_])();

	// ���C�̕�����ݒ�
	eyeCol_->SetDir(forwardVec_);

	float3 pos = object_->GetPosition();

	ImGui::Begin("Player");
	ImGui::Text("Pos = {%f, %f, %f}", pos.x, pos.y, pos.z);
	ImGui::Text("ForwardVec = {%f, %f, %f}", forwardVec_.x, forwardVec_.y, forwardVec_.z);
}

void Player::DrawObject3D()
{
	//object_->Draw();
	//testObj_->Draw();

	for (auto& it : bullets_) it->Draw();
}

void Player::DrawSprite()
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

	// ����q���g�`��
	//if (isHitItem) opeTips_->Draw(opeTipsHandle_);
}

void Player::ObjUpdate()
{
	object_->Update();
	testObj_->Update();

	camera_->Update();
}

void Player::OnCollision()
{

	//if (legCol_->GetIsHit() && isGround_ == false) {
	//	float3 reject = legCol_->GetReject();
	//	camera_->SetEye(camera_->GetEye() + reject);
	//	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
	//	object_->SetPosition(camera_->GetEye());
	//	gravity_ = 0.0f;
	//	state_ = NORMAL;
	//	isGround_ = true;
	//}

	//if (legCol_->GetIsHit()) {
	//	isGround_ = true;
	//	state_ = NORMAL;
	//	gravity_ = 0.0f;
	//	float3 reject = legCol_->GetReject();
	//	camera_->SetEye(camera_->GetEye() + Vector3{0.0f, reject.y, 0.0f});
	//	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
	//	object_->SetPosition(camera_->GetEye());
	//}

	//else {
	//	isGround_ = false;
	//	state_ = AIR;
	//}

	if (climbCol_->GetIsHit() && key_->PushKey(DIK_W)) {
		state_ = CLIMB;
	}

	else if (legCol_->GetIsHit()) {
		isGround_ = true;
		state_ = NORMAL;
		gravity_ = 0.0f;
		float3 reject = legCol_->GetReject();
		camera_->SetEye(camera_->GetEye() + Vector3{0.0f, reject.y, 0.0f});
		camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
		object_->SetPosition(camera_->GetEye());
	}

	else {
		isGround_ = false;
		state_ = AIR;
	}

	if (key_->PushKey(DIK_LSHIFT) && key_->PushKey(DIK_W)) {
		isDash_ = true;
	}

	else {
		isDash_ = false;
	}

	if (playerCol_->GetIsHit()) {
		float3 reject = playerCol_->GetReject();
		camera_->SetEye(camera_->GetEye() + reject);
		camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
		//// �Ǔo��p�R���C�_�[�𐳖ʂɐݒ�
		//float3 climbColPos = {
		//	forwardVec_.x * 1.25f,
		//	0.0f,
		//	forwardVec_.z * 1.25f
		//};
		//climbCol_->SetOffset(climbColPos);
		//object_->SetPosition(camera_->GetEye());
	}

	bool isBossGen = false;

	if (eyeCol_->GetIsHit()) {
		if (eyeCol_->GetCollider()->GetAttribute() == COL_BOSSGENERATOR) {
			isBossGen = true;
		}
	}

	if (isBossGen && key_->TriggerKey(DIK_F)) {
		enemyMgr_->SummonBoss();
	}

	ImGui::Text("IsHit = %d", isBossGen);
	ImGui::Text("Gravity = %f", gravity_);
	ImGui::Text("State = %s", stateName_[state_].c_str());
	ImGui::End();
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
	gravity_ = Util::Clamp(gravity_, maxGravity_, -1.0f);
	camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
	object_->SetPosition(camera_->GetEye());

	// ���_����
	EyeMove();

	// �ړ�����
	Move();
}

void Player::Shoot()
{
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		// �e�̍X�V
		(*it)->Update();

		// �e�̐����t���O��[OFF]�ɂȂ�����e���폜
		if ((*it)->GetIsAlive() == false) it = bullets_.erase(it);
		else ++it;
	}

	// �Ō�ɒe�������Ă���̌o�ߎ���
	float result = (Util::GetTimrMil() - shotTime_) / 1000.0f;

	// �c�e����0�ȉ��Ȃ炱�̌�̏������΂�
	if (nowBullet_ <= 0) return;

	// �}�E�X�����N���b�N���Ă��Ȃ������炱�̌�̒e���΂�
	if (mouse_->PushMouseButton(MouseButton::M_LEFT) == false) return;

	// �o�ߎ��Ԃ��w�莞�Ԃ��߂��Ă��Ȃ������珈�����΂�
	if (!(result >= shotInterval_)) return;

	// �e�����������Ԃ��L�^
	shotTime_ = Util::GetTimrMil();

	// �c�e�����炷
	nowBullet_--;

	// �e�𐶐�
	bullets_.emplace_back(std::make_unique<Bullet>(mBullet_.get(), BulletType::PLAYER, camera_->GetEye(), forwardVec_));
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
		inputVec.z* forwardVec_.x,
		0.0f,
		inputVec.z* forwardVec_.z
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

	// �Ǔo��p�R���C�_�[�𐳖ʂɐݒ�
	//float3 climbColPos = {
	//	forwardVec_.x * 1.25f,
	//	0.0f,
	//	forwardVec_.z * 1.25f
	//};
	//climbCol_->SetOffset(forwardVec_ * 1.25f);
	testObj_->SetPosition(object_->GetPosition() + forwardVec_ * 1.25f);

	// �I�u�W�F�N�g�̍X�V
	object_->SetPosition(camera_->GetEye());
}

void Player::EyeMove()
{
	// ���_�p�x
	static float3 eyeAngle = { 0.0f, 90.0f, 0.0f };

	// �}�E�X�̈ړ��ʕ����_�̊p�x�ɉ��Z
	eyeAngle.x += mouse_->GetMouseVelosity().x * sencivity_;
	eyeAngle.y += mouse_->GetMouseVelosity().y * sencivity_;

	// ���_�ړ��̏㉺�ɐ�����t����
	eyeAngle.y = Util::Clamp(eyeAngle.y, 180.0f, 0.0f);

	// �O���x�N�g���v�Z
	forwardVec_ = {
		sinf(Util::Degree2Radian(eyeAngle.x)),
		cosf(Util::Degree2Radian(eyeAngle.y)),
		cosf(Util::Degree2Radian(eyeAngle.x))
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
}

void Player::Jump()
{
	// [SPACE]�L�[����������W�����v����
	//if (isGround_ && key_->TriggerKey(DIK_SPACE)) {
	//	gravity_ = -jumpSpd_;
	//	isGround_ = false;
	//	camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	//	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
	//	object_->SetPosition(camera_->GetEye());
	//}

	if (key_->TriggerKey(DIK_SPACE)) {
		gravity_ = -jumpSpd_;
		camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
		camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
		object_->SetPosition(camera_->GetEye());
	}
}

void Player::Fall()
{
	//// �n�ʂɒ����Ă��Ȃ��Ȃ痎������������
	//if (isGround_ == false) {
	//	gravity_ += gAcc_;// �d�͉����x�����Z
	//	gravity_ = Util::Clamp(gravity_, maxGravity_, -1000.0f);
	//	camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	//	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
	//	object_->SetPosition(camera_->GetEye());
	//}

	gravity_ += gAcc_;// �d�͉����x�����Z
	gravity_ = Util::Clamp(gravity_, maxGravity_, -1000.0f);
	camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
	object_->SetPosition(camera_->GetEye());

	//// �J�����̍��W��2.0f�ȉ��ɂȂ�Ȃ��悤��
	//float3 eye = camera_->GetEye();
	//eye.y = Util::Clamp(eye.y, 1000.0f, 2.0f);

	//// �J�����̍��W��2.0�ȉ��ɂȂ�����n�ʂɒ��������Ƃɂ���
	//if (eye.y <= 2.0f) {
	//	isGround_ = true;
	//	state_ = NORMAL;
	//}

	//// �J�����̍��W�X�V
	//camera_->SetEye(eye);
}

void Player::Dash()
{
	static float time = 1.0f;

	if (isDash_) fovAngleY_ += 2.0f;
	else fovAngleY_ -= 2.0f;

	fovAngleY_ = Util::Clamp(fovAngleY_, 80.0f, 70.0f);

	camera_->SetFovAngleY(fovAngleY_);
}