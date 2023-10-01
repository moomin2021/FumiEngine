#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Sprite.h"
#include "Camera.h"

#include "PointCollider.h"

#include <memory>

class TitleScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	Key* key_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// �X�v���C�g
	std::unique_ptr<Sprite> sTitle_ = nullptr;

	// �e�N�X�`���n���h��
	uint16_t hTitle_ = 0;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	void OnCollision();

	void MatUpdate();
#pragma endregion
};