#pragma once
#include "BaseScene.h"// -> �e�V�[���̃x�[�X
#include "Key.h"// -> �L�[�{�[�h����

class SceneManager {
	// --�����o�ϐ�-- //
public:

private:
	// �L�[�{�[�h���̓N���X
	Key* key_;

	// ���݂̃V�[��
	BaseScene* nowScene_;

	// --�����o�֐�-- //
public:
	// �C���X�^���X�擾
	static SceneManager* GetInstance();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �R���X�g���N�^
	SceneManager();

	// �f�X�g���N�^
	~SceneManager();

	// --�֎~-- //
public:
	// �R�s�[�R���X�g���N�^
	SceneManager(const SceneManager& instance) = delete;

	// ������Z�q
	SceneManager& operator=(const SceneManager& instance) = delete;
};