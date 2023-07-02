#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Camera.h"
#include "Bullet.h"

#include <memory>

class Player
{
#pragma region �����o�ϐ�
private:
	// ���̓N���X�C���X�^���X
	Key* key_;		// �L�[�{�[�h
	Mouse* mouse_;	// �}�E�X

	// ���_�J����
	std::unique_ptr<Camera> camera_;

	// �e�̃��f��
	std::unique_ptr<Model> mBullet_;

	// �e�̃N���X
	std::vector <std::unique_ptr<Bullet>> bullets_;

	// �����x�N�g��
	Vector3 forwardVec_;// �O��
	Vector3 rightVec_;	// �E

	// ���x
	float moveSpd_;			// �ړ����x
	float cameraAngleSpd_;	// �J�����̊p�x�̈ړ����x
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
};