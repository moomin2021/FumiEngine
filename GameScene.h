#pragma once
#include "BaseScene.h"// -> �V�[���̃x�[�X
#include "Sprite.h"// -> �X�v���C�g
#include "Key.h"// -> �L�[�{�[�h����

class GameScene : public BaseScene {
	// --�����o�ϐ�-- //
public:

private:
	// �L�[�{�[�h���̓N���X
	Key* key_;

	// �X�v���C�g
	Sprite* sprite_;

	// �`��t���O
	bool isDraw_;

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