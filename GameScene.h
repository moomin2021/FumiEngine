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
	// �L�[�{�[�h����
	Key* key_;

	// ���f��
	Model* blackFloor_;// -> ���F�̏�
	Model* whiteFloor_;// -> ���F�̏�
	
	// �e�I�u�W�F�N�g�̐�
	static const size_t maxFloor_ = 100;

	// �I�u�W�F�N�g
	Object3D* floor_[maxFloor_];// -> ��

	// �J����
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