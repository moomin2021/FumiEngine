#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Camera.h"
#include "Bullet.h"
#include "Sprite.h"
#include "RayCollider.h"

#include <memory>

class Player
{
#pragma region �����o�ϐ�
public:
	// �e�̃N���X
	std::vector <std::unique_ptr<Bullet>> bullets_;

private:
	// ���̓N���X�C���X�^���X
	Key* key_ = nullptr;		// �L�[�{�[�h
	Mouse* mouse_ = nullptr;	// �}�E�X

	// ���_�J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// �e�̃��f��
	std::unique_ptr<Model> mBullet_ = nullptr;

	// ���C�̃R���C�_�[
	std::unique_ptr<RayCollider> eyeCollider_ = nullptr;// ���_�̃��C

	// �����x�N�g��
	Vector3 forwardVec_ = {};// �O��
	Vector3 rightVec_ = {};// �E

	// ���x
	float moveAcc_ = 0.1f;			// �ړ������x
	float maxSpd_ = 1.5f;			// �ő呬�x
	float moveSpd_ = 0.0f;			// �ړ����x
	float cameraAngleSpd_ = 0.1f;	// �J�����̊p�x�̈ړ����x

	// �N���X�w�A
	uint16_t crossHairHandle_ = 0;
	std::unique_ptr<Sprite> sCrossHair_ = nullptr;

	// �W�����v�p
	bool isGround_ = false;	// �n�ʂɂ��Ă��邩
	float gravity_ = 0.0f;	// �d��
	float gAcc_ = 0.2f;		// �d�͉����x
	float jumpSpd_ = 2.5f;	// �W�����v���x
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// �X�V����
	void Update();

	// �I�u�W�F�N�g3D�`�揈��
	void Object3DDraw();

	// �O�ʃX�v���C�g�`��
	void FrontSpriteDraw();

private:
	// �e��������
	void Shoot();

	/// <summary>
	/// ���_�ړ�����
	/// </summary>
	void EyeMove();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	// �R���C�_�[�̍X�V
	void ColliderUpdate();

	// �Փˎ��̏���
	void OnCollision();
#pragma endregion

#pragma region �Q�b�^�[�֐�

#pragma endregion
};