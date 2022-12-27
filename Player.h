#pragma once
#include "Vector3.h"
#include "Camera.h"

class Player {
	// �����o�ϐ�
public:

private:
	// �����̍��W
	Vector3 legPos_;

	// �J����
	Camera* camera_;

	// �����o�֐�
public:
	// �R���X�g���N�^
	Player();

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
	// �f�X�g���N�^
	~Player();

};