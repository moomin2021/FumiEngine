#pragma once
#include "LoadBlender.h"
#include "BaseScene.h"
#include "Sprite.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
#include <vector>
#include <memory>

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_;

	// �u�����_�[����ǂݍ��񂾃V�[��
	std::unique_ptr<LoadBlender> loadBle_;

	// �J����
	std::unique_ptr<Camera> camera_;

	// ���C�g�O���[�v
	std::unique_ptr<LightGroup> lightGroup_;
	std::unique_ptr<DirectionalLight> dirLight_;

	std::unique_ptr<Model> mCube_;

	std::unique_ptr<Object3D> oCube_;

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

