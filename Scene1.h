#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Key.h"

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_;

	// �J����
	Camera* camera_;

	// ���f��
	Model* sphereM_;

	// �I�u�W�F�N�g
	Object3D* sphereObj_[2];

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene1();

	// �f�X�g���N�^
	~Scene1();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
};

