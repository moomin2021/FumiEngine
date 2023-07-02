#pragma once
#include "BaseScene.h"
#include "LightGroup.h"
#include "LoadStage.h"
#include "Player.h"

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// ���C�g
	std::unique_ptr<LightGroup> lightGroup_;
	std::unique_ptr<DirectionalLight> dirLight_;

	// �X�e�[�W�ǂݍ��ݗp�N���X
	std::unique_ptr<LoadStage> loadStage_;

	// �v���C���[
	std::unique_ptr<Player> player_;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene1();

	// �f�X�g���N�^
	~Scene1();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
};

