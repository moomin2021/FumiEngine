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
	Key* key_;		// �L�[�{�[�h
	Mouse* mouse_;	// �}�E�X

	// ���_�J����
	std::unique_ptr<Camera> camera_;

	// �e�̃��f��
	std::unique_ptr<Model> mBullet_;

	// ���C�̃R���C�_�[
	std::unique_ptr<RayCollider> eyeCollider_;// ���_�̃��C
	std::unique_ptr<RayCollider> downCollider_;// �^���Ɍ����Ă���n�ʌ��o�p�̃��C

	// �����x�N�g��
	Vector3 forwardVec_;// �O��
	Vector3 rightVec_;	// �E

	// ���x
	float moveAcc_;			// �ړ������x
	float maxSpd_;// �ő呬�x
	float moveSpd_;
	float cameraAngleSpd_;	// �J�����̊p�x�̈ړ����x

	// �N���X�w�A
	uint16_t crossHairHandle_;
	std::unique_ptr<Sprite> sCrossHair_;

	// �W�����v�p
	bool isGround_;// �n�ʂɂ��Ă��邩
	float gravity_;// �d��
	float gAcc_;// �d�͉����x
	float jumpSpd_;// �W�����v���x
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �v���C���[
	/// </summary>
	Player();

	// �f�X�g���N�^
	~Player();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// 2D�`�揈��
	/// </summary>
	void Draw2D();

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