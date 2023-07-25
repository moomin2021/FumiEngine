#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Sprite.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "RayCollider.h"

#include <vector>
#include <memory>

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_;

	// �J����
	std::unique_ptr<Camera> camera_;

	// ���C�g
	std::unique_ptr<LightGroup> lightGroup_;
	std::unique_ptr<DirectionalLight> dirLight_;
	std::unique_ptr<PointLight> pointLight_;

	// ���f��
	std::unique_ptr<Model> mFloor_;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> oFloor_;

	// �e�N�X�`���n���h��
	uint16_t mainTexHandle_;
	uint16_t subTexHandle_;
	uint16_t maskTexHandle_;

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

