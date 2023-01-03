#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Camera.h"
#include "Key.h"
#include "Mouse.h"

struct Circle {
	float x, y;// -> ���W
	float r;// -> ���a
};

class Player {
	// �����o�ϐ�
public:
	Circle col_;// -> �����蔻��p�f�[�^
	Circle oldCol_;// -> 1�t���[���O�̃f�[�^

private:
	Key* key_;// -> �L�[�{�[�h����
	Mouse* mouse_;// -> �}�E�X����
	Camera* camera_;// -> �J����

	float angleX_, angleY_;// -> �J�����A���O��
	float cameraSens_;// -> �J�������x
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