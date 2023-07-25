#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Key.h"
#include "Dissolve3D.h"

#include <memory>

class Scene3 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// ���C�g
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// ���f��
	std::unique_ptr<Model> model_;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> object_;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene3();

	// �f�X�g���N�^
	~Scene3();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
};

