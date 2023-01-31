#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
#include "CollisionPrimitive.h"

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_;

	// �J����
	Camera* camera_;

	// ���C�g
	LightGroup* lightGroup_;

	// ���f��
	Model* sphereM_;
	Model* floorM_;

	// �I�u�W�F�N�g
	Object3D* object_[2];

	// �����蔻�� ��
	Sphere sphereC_;

	// �����蔻�� ����
	Plane planeC_;

	// �J�E���g
	int count_;

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

