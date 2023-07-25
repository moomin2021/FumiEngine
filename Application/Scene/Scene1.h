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
	std::unique_ptr<Model> mCube_;
	std::unique_ptr<Model> mSphere_;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> oFloor_;
	std::unique_ptr<Object3D> oSphere_;
	std::vector<std::unique_ptr<Object3D>> oCube_;

	std::unique_ptr<SphereCollider> sphereCollider_;
	std::unique_ptr<MeshCollider> meshCollider_;
	std::unique_ptr<RayCollider> rayCollider_;

	// �e�N�X�`���n���h��
	uint16_t haeHandle_;
	uint16_t mainTexHandle_;
	uint16_t subTexHandle_;
	uint16_t maskTexHandle_;

	// �X�v���C�g
	std::unique_ptr<Sprite> sHae_;

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

