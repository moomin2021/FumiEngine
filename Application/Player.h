#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Camera.h"
#include "Bullet.h"
#include "Sprite.h"

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

	// �����x�N�g��
	Vector3 forwardVec_;// �O��
	Vector3 rightVec_;	// �E

	// ���x
	float moveSpd_;			// �ړ����x
	float cameraAngleSpd_;	// �J�����̊p�x�̈ړ����x

	// �N���X�w�A
	uint16_t crossHairHandle_;
	std::unique_ptr<Sprite> sCrossHair_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �v���C���[
	/// </summary>
	Player();

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
#pragma endregion

#pragma region �Q�b�^�[�֐�

#pragma endregion
};