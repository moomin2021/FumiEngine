#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Key.h"
#include "Sprite.h"
#include <memory>

class ClearScene : public BaseScene {
	// --�����o�ϐ�-- //
public:

private:
	// �L�[�{�[�h����
	Key* key_;

	// �J����
	Camera* camera_;

	std::unique_ptr<Sprite> clearS_;
	int clearH_;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	ClearScene();

	// �f�X�g���N�^
	~ClearScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:

};