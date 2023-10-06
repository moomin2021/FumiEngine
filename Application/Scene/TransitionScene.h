#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Camera.h"

#include <memory>

class TransitionScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	std::unique_ptr<Camera> camera_ = nullptr;

	uint16_t maxTimr_ = 60;
	uint16_t time_ = 0;

	std::unique_ptr<Sprite> sLoad_ = nullptr;

	uint16_t loadHandle_ = 0;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	TransitionScene();

	// �f�X�g���N�^
	~TransitionScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ�����
	void OnCollision();

	// �s��X�V
	void MatUpdate();
#pragma endregion
};