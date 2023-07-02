#include "Player.h"
#include "Object3D.h"
#include "float3.h"
#include "Vector3.h"
#include "Util.h"

Player::Player() :
#pragma region ���������X�g
	// ���̓N���X�C���X�^���X
	key_(nullptr),
	mouse_(nullptr),

	// ���_�J����
	camera_(nullptr),

	// �����x�N�g��
	forwardVec_{},	// �O��
	rightVec_{},	// �E

	// ���x
	moveSpd_(1.0f),// �ړ����x
	cameraAngleSpd_(1.0f)// �J�����̊p�x�̈ړ����x
#pragma endregion
{
	// ���̓N���X�C���X�^���X�擾
	key_	= Key::GetInstance();	// �L�[�{�[�h
	mouse_	= Mouse::GetInstance();	// �}�E�X

	// �J�����𐶐����ݒ�
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// �J������K�p
	Object3D::SetCamera(camera_.get());
}

void Player::Update()
{
	// ���_�ړ�
	EyeMove();

	// �ړ�
	Move();

	// �J�����̍X�V
	camera_->Update();
}

void Player::Draw()
{
}

void Player::EyeMove()
{
	// ���_�̊p�x
	static float3 angle = { 0.0f, 90.0f, 0.0f };

	// �}�E�X�̈ړ��ʕ����_�̊p�x�ɉ��Z
	angle.x += mouse_->GetMouseVelosity().x * cameraAngleSpd_;
	angle.y += mouse_->GetMouseVelosity().y * cameraAngleSpd_;

	// ���_�ړ��̏㉺�ɐ�����t����
	angle.y = Util::Clamp(angle.y, 180.0f, 0.0f);

	// �O���x�N�g���v�Z
	forwardVec_ = {
		sinf(Util::Degree2Radian(angle.x)),
		cosf(Util::Degree2Radian(angle.y)),
		cosf(Util::Degree2Radian(angle.x))
	};

	// �O���x�N�g�����K��
	forwardVec_.normalize();

	// �E�x�N�g���̌v�Z
	rightVec_ = Vector3(forwardVec_.x, 0.0f, forwardVec_.z);
	rightVec_ = -rightVec_.cross(rightVec_ + Vector3(0.0f, 1.0f, 0.0f));

	// �E�x�N�g�����K��
	rightVec_.normalize();

	// �J�����̒����_��ݒ�
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
}

void Player::Move()
{
	// �ړ�����
	Vector3 moveVec = {
		static_cast<float>(key_->PushKey(DIK_D) - key_->PushKey(DIK_A)),
		0.0f,
		static_cast<float>(key_->PushKey(DIK_W) - key_->PushKey(DIK_S))
	};

	float3 forwardMove = {
		moveVec.z * forwardVec_.x * moveSpd_,
		0.0f,
		moveVec.z * forwardVec_.z * moveSpd_
	};

	float3 rightMove = {
		moveVec.x * rightVec_.x * moveSpd_,
		0.0f,
		moveVec.x * rightVec_.z * moveSpd_
	};

	camera_->SetEye(camera_->GetEye() + (forwardMove + rightMove));

	// �J�����̒����_��ݒ�
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
}