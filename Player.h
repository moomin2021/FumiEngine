#pragma once
#include "Vector3.h"
#include "Camera.h"
#include "Key.h"
#include "Mouse.h"

class Player {
	// �����o�ϐ�
public:
	Vector3 oldPos_;// -> �O�t���[���̍��W

private:
	Key* key_;// -> �L�[�{�[�h����
	Mouse* mouse_;// -> �}�E�X����
	Camera* camera_;// -> �J����

	// �J�����A���O��
	float angleX_, angleY_;

	// �J�������x
	float cameraSens_;

	float moveSpeed_;// -> �ړ����x

	Vector3 forwardVec_;// -> �O���x�N�g��
	Vector3 rightVec_;// -> �E�x�N�g��

	// �����o�֐�
public:
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

#pragma region �Z�b�^�[
	// �J������ݒ�
	void SetCamera(Camera* camera) { camera_ = camera; }
#pragma endregion

private:
	// ���_�ړ�
	void EyeMove();

	// �ړ�����
	void Move();
};