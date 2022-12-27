#pragma once
#include "BaseScene.h"// -> �x�[�X�V�[���N���X
#include "Sprite.h"// -> �X�v���C�g
#include "Model.h"// -> ���f���N���X
#include "Camera.h"// -> �J�����N���X
#include "Object3D.h"// -> �I�u�W�F�N�g3D
#include "Key.h"// -> �L�[�{�[�h����
#include "Player.h"// -> �v���C���[�N���X

class GameScene : public BaseScene {
	// --�����o�ϐ�-- //
public:

private:
	// �L�[�{�[�h����
	Key* key_;

	// �J����
	Camera* camera_;

	// �v���C���[
	Player* player_;

	// ���f��
	Model* blackFloor_;// -> ���F�̏�
	Model* whiteFloor_;// -> ���F�̏�
	
	// �e�I�u�W�F�N�g�̐�
	static const size_t maxFloor_ = 100;

	// �I�u�W�F�N�g
	Object3D* floor_[maxFloor_];// -> ��

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