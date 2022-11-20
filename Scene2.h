#pragma once
// --�e�V�[���̌p���N���X-- //
#include "BaseScene.h"

// --�J�����N���X-- //
#include "Camera.h"

// --�I�u�W�F�N�g3D�N���X-- //
#include "Object3D.h"

// --�r���{�[�h�N���X-- //
#include "BillBoard.h"

// --�L�[�{�[�h���̓N���X-- //
#include "Key.h"

class Scene2 : public BaseScene {
	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene2();

	// �f�X�g���N�^
	~Scene2();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:

	// --�����o�ϐ�-- //
public:

private:
	// �L�[�{�[�h
	Key* key_;

	// �J����
	Camera* camera_;

	// �r���{�[�h
	BillBoard* billBoard_[10];

	// �摜�n���h��
	int valoHandle_;
};