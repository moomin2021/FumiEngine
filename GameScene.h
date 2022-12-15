#pragma once
#include "BaseScene.h"// -> �V�[���̃x�[�X
#include "Sprite.h"// -> �X�v���C�g
#include "Model.h"// -> ���f���N���X
#include "Camera.h"
#include "Object3D.h"

class GameScene : public BaseScene {
	// --�����o�ϐ�-- //
public:

private:
	// �X�v���C�g
	Sprite* sprite_;

	Model* model_;

	Camera* camera_;

	int textureHandle_;

	Object3D* object_;

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