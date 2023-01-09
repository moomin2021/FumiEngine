#pragma once
#include "Object3D.h"
#include "Model.h"
#include <vector>
#include "Camera.h"
#include "ColType.h"

class Stage {
	// �����o�ϐ�
public:
	std::vector<Object3D> wallsObj_;// ��
	std::vector<Line2D> wallsCol_;// �Ǔ����蔻��
private:
	int area_[27][27];

	Model* wallM_;

	Camera* camera_;

	// �����o�֐�
public:
	// �R���X�g���N�^
	Stage();

	// �f�X�g���N�^
	~Stage();

	// ����������
	void Initialize();

	// �`�揈��
	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; }

private:

};