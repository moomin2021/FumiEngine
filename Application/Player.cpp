#include "Player.h"
#include "CollisionAttribute.h"
#include "Util.h"

#include <imgui_impl_DX12.h>

Player::Player()
{
}

Player::~Player()
{
	colMgr_->RemoveCollider(playerCol_.get());
	colMgr_->RemoveCollider(legCol_.get());
}

void Player::Initialize()
{
#pragma region �C���X�^���X
	key_ = Key::GetInstance();
	mouse_ = Mouse::GetInstance();
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -10.0f });
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ���f��
	mSphere_ = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region �I�u�W�F�N�g
	oPlayer_ = std::make_unique<Object3D>(mSphere_.get());
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
#pragma endregion
}

void Player::Update()
{
	// ��ԕʍX�V����
	(this->*stateTable[state_])();
}

void Player::Draw3D()
{

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
}

void Player::MatUpdate()
{
	// �J����
	camera_->Update();

	// �I�u�W�F�N�g
	oPlayer_->MatUpdate();
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

	// �W�����v����
	Jump();
}

void Player::Air()
{
	// ���_����
	EyeMove();

	// �ړ�����
	Move();

	// ��������
	Fall();
}

void Player::Climb()
{
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