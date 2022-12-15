#pragma once
#include "BaseScene.h"
#include "Sprite.h"// -> �X�v���C�g
#include "Model.h"// -> ���f���N���X
#include "Camera.h"
#include "Object3D.h"
#include "Key.h"

class GameScene : public BaseScene {
	// --�����o�ϐ�-- //
public:

private:
	Key* key_;

	Model* player_;
	Model* cube_;

	Camera* camera_;

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