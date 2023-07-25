#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Key.h"
#include "Dissolve3D.h"

#include <memory>

class Scene2 : public BaseScene
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

	// �f�B�]���u
	std::unique_ptr<Dissolve3D> dissolve_ = nullptr;

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

