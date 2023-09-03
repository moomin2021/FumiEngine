#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Mouse.h"
#include "PointCollider.h"
#include "BoxCollider.h"

#include <memory>

class Scene3 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;

	std::unique_ptr<PointCollider> pCol_ = nullptr;
	std::unique_ptr<BoxCollider> boxCol_ = nullptr;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene3();

	// �f�X�g���N�^
	~Scene3();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
};

