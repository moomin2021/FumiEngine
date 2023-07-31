#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "Sprite.h"
#include "Key.h"

#include <memory>

class GameOverScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	// �L�[�{�[�h����
	Key* key_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// �X�v���C�g
	std::unique_ptr<Sprite> sGameOver_ = nullptr;

	// �e�N�X�`��
	uint16_t gameOverHandle_ = 0;

#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	GameOverScene();

	// �f�X�g���N�^
	~GameOverScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
#pragma endregion
};