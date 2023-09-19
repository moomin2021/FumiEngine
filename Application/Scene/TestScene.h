#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "Model.h"
#include "Object3D.h"
#include "LightGroup.h"
#include "DirectionalLight.h"

#include <memory>

class TestScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// ���C�g�O���[�v
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	// ���s����
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// ���f��
	std::unique_ptr<Model> model_ = nullptr;

	// �I�u�W�F�N�g3D
	std::unique_ptr<Object3D> object_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	TestScene();

	// �f�X�g���N�^
	~TestScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �Փˎ�����
	void OnCollision();

	// �s��X�V����
	void MatUpdate();
#pragma endregion
};