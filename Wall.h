#pragma once
#include "Vector3.h"

class Wall {
	// �����o�ϐ�
public:
	Vector3 pos_;// -> ���W

private:

	// �����o�֐�
public:
	// �R���X�g���N�^
	Wall();

	// �f�X�g���N�^
	~Wall();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:

};