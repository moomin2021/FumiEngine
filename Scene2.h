#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Sprite.h"
#include "Key.h"
#include <memory>

class Scene2 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_;

	// �J����
	std::unique_ptr<Camera> camera_;

	// �X�v���C�g
	std::unique_ptr<Sprite> sprite_[2];

	// �e�N�X�`���n���h��
	int texHandle_[2];

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
};

