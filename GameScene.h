#pragma once
#include "BaseScene.h"// -> �V�[���̃x�[�X
#include "Sprite.h"// -> �X�v���C�g

class GameScene : public BaseScene {
	// --�����o�ϐ�-- //
public:

private:
	// �X�v���C�g
	Sprite* sprite_;

	// --�����o�֐�-- //
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

};