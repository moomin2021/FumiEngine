#pragma once
#include "Object3D.h"
#include "Model.h"
#include <vector>
#include "Camera.h"
#include "ColType.h"
#include "Door.h"

class Stage {
	// �����o�ϐ�
public:
	std::vector<Object3D> wallsObj_;// ��
	std::vector<Line2D> wallsCol2D_;// 2D�p�����蔻��
	std::vector<Board> wallsCol3D_;// 3D�p�����蔻��

	std::vector<Door> doors_;

	//std::vector<Object3D> doorsObj_;
	//std::vector<Line2D> doorsCol2D_;
	//std::vector<Door> doorsCol3D_;

	//std::vector<Door> doors_;
private:
	int area_[27][27];

	Model* wallM_;
	Model* doorM_;// -> �h�A���f��

	Camera* camera_;

	// �����o�֐�
public:
	// �R���X�g���N�^
	Stage();

	// �f�X�g���N�^
	~Stage();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; }

private:

};