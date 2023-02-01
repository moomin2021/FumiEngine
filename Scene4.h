#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
#include "CollisionPrimitive.h"

class Scene4 : public BaseScene
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
	Model* triangleM_;

	// �I�u�W�F�N�g
	Object3D* object_[3];

	// �����蔻�� ���C
	Ray rayC_;

	// �����蔻�� �O�p�`
	Triangle triangleC_;

	// �J�E���g
	int count_;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene4();

	// �f�X�g���N�^
	~Scene4();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
};

