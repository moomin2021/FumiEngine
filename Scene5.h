#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
#include "CollisionPrimitive.h"

class Scene5 : public BaseScene
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
	Model* rayM_;

	// �I�u�W�F�N�g
	Object3D* object_[3];

	// �����蔻�� ���C
	Ray rayC_;

	// �����蔻�� ��
	Sphere sphereC_;

	// �J�E���g
	int count_;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene5();

	// �f�X�g���N�^
	~Scene5();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
};

