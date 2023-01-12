#pragma once
#include "Object3D.h"
#include "Model.h"
#include "ColType.h"
#include <memory>

class Door
{
	// �����o�ϐ�
public:
	std::unique_ptr<Object3D> object_;
	//Object3D object_;// -> �I�u�W�F�N�g
	Vector3 oldPos_;

	Line2D col2D_;// -> 2D�p�����蔻��
	Board col3D_;// -> 3D�p�����蔻��

	bool isRight_;// -> �h�A���E�ɂ��邩
	bool isMove_;

private:
	int timer_;
	int maxTime_;

	// �����o�֐�
public:
	Door();// --> �R���X�g���N�^
	~Door();// -> �f�X�g���N�^
	void Initialize();
	void Update();// -----> �X�V����
	void Draw();// -------> �`�揈��

	void SetModel(Model* model) { object_->SetModel(model); }
	void SetCamera(Camera* camera) { object_->SetCamera(camera); }
};

