#pragma once
#include "BaseScene.h"
#include "Camera.h"

#include <memory>

class TestScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	TestScene();

	// �f�X�g���N�^
	~TestScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �Փˎ�����
	void OnCollision();

	// �s��X�V����
	void MatUpdate();
#pragma endregion
};