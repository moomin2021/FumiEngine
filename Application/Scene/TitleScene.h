#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Mouse.h"
#include "Sprite.h"
#include "Camera.h"

#include "CollisionManager2D.h"
#include "PointCollider.h"
#include "BoxCollider.h"

#include <memory>

class TitleScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;
	CollisionManager2D* colMgr2D_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// �X�v���C�g
	std::unique_ptr<Sprite> sTitle_ = nullptr;

	// �e�N�X�`���n���h��
	uint16_t hTitle_ = 0;

	// �X�v���C�g
	std::vector<std::unique_ptr<Sprite>> sSelectButtons_ = {};
	std::unique_ptr<Sprite> sSelectButtonFrame_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sSelectText_ = {};

	// �摜�n���h��
	uint16_t gSelectButton_ = 0;
	uint16_t gSelectButtonFrame_ = 0;
	std::vector<uint16_t> gSelectText_ = {};

	// �R���C�_�[
	std::unique_ptr<PointCollider> mouseCol_ = nullptr;
	std::vector<std::unique_ptr<BoxCollider>> selectButtonsCol_ = {};
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