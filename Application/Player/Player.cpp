#include "Player.h"

#include <imgui_impl_DX12.h>

Player::~Player()
{
	// �폜
	colMgr_->RemoveCollider(playerCol_.get());
}

void Player::Initialize()
{
#pragma region �C���X�^���X�擾
	key_	= Key::GetInstance();	// �L�[�{�[�h
	mouse_	= Mouse::GetInstance();	// �}�E�X
	colMgr_ = CollisionManager::GetInstance();// �R���W�����}�l�[�W���[
#pragma endregion

#pragma region �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 2.0f, 0.0f });
#pragma endregion

#pragma region ���f��
	model_ = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region �I�u�W�F�N�g
	object_ = std::make_unique<Object3D>(model_.get());
#pragma endregion

#pragma region �R���C�_�[
	playerCol_ = std::make_unique<SphereCollider>();
	playerCol_->LinkObject3D(object_.get());
	colMgr_->AddCollider(playerCol_.get());// �ǉ�
#pragma endregion

	// �J�������I�u�W�F�N�g3D�ɓK�p
	Object3D::SetCamera(camera_.get());
}

void Player::Update()
{
	EyeMove();
	Move();

	float3 pos = camera_->GetEye();

	ImGui::Begin("Player");
	ImGui::Text("Pos = {%f, %f, %f}", pos.x, pos.y, pos.z);
	ImGui::Text("ForwardVec = {%f, %f, %f}", forwardVec_.x, forwardVec_.y, forwardVec_.z);
	ImGui::End();
}

void Player::Draw()
{
	//object_->Draw();
}

void Player::ObjUpdate()
{
	object_->Update();

	camera_->Update();
}

void Player::OnCollision()
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
	moveSpd_ = Util::Clamp(moveSpd_, maxMoveSpd_, 0.0f);

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