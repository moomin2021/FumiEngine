#pragma once
#include "BaseScene.h"// -> �x�[�X�V�[���N���X
#include "Sprite.h"// -> �X�v���C�g
#include "Model.h"// -> ���f���N���X
#include "Camera.h"// -> �J�����N���X
#include "Object3D.h"// -> �I�u�W�F�N�g3D
#include "Key.h"// -> �L�[�{�[�h����
#include "Player.h"// -> �v���C���[�N���X
#include "Stage.h"
#include "Sprite.h"

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

	// �X�e�[�W
	Stage* stage_;
	
	// �e�I�u�W�F�N�g�̐�
	static const size_t maxFloor_ = 169;

	int openOrCloseH_;
	Sprite* openOrClose_;
	bool isText_;

	std::unique_ptr<Sprite> hintText_;
	int hintTextH_;

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
	// �����蔻�菈��
	void Collision();

	void WallCol();// -> �ǂƃv���C���[�̓����蔻��
	void WallSlide(Line2D& wall);// -> �ǂ��菈��

	void DoorCol();

};