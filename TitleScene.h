#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Key.h"
#include "Sprite.h"
#include <memory>

class TitleScene : public BaseScene {
	// --�����o�ϐ�-- //
public:

private:
	// �L�[�{�[�h����
	Key* key_;

	std::unique_ptr<Sprite> titleS_;
	int titleImage_;

	std::unique_ptr<Sprite> spaceStartText_;
	int spaceStartTextH_;

	// �J����
	Camera* camera_;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:

};