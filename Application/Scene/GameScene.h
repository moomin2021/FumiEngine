#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "LightGroup.h"
#include "DirectionalLight.h"

#include "Stage.h"
#include "Player.h"

#include <memory>

class GameScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	// ���C�g�O���[�v
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	// ���s����
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// �X�e�[�W�I�u�W�F�N�g�̊Ǘ��N���X
	std::unique_ptr<Stage> stage_ = nullptr;

	// �v���C���[
	std::unique_ptr<Player> player_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	GameScene();

	// �f�X�g���N�^
	~GameScene();

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