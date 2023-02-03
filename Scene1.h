#pragma once
<<<<<<< HEAD:GameScene.h
#include "BaseScene.h"// -> �x�[�X�V�[���N���X
#include "Sprite.h"// -> �X�v���C�g
#include "Model.h"// -> ���f���N���X
#include "Camera.h"// -> �J�����N���X
#include "Object3D.h"// -> �I�u�W�F�N�g3D
#include "Key.h"// -> �L�[�{�[�h����
#include "Player.h"// -> �v���C���[�N���X
#include "Stage.h"
#include "Sprite.h"
=======
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
>>>>>>> master:Scene1.h

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_;

	// �J����
	Camera* camera_;

<<<<<<< HEAD:GameScene.h
	// �v���C���[
	Player* player_;

	// �X�e�[�W
	Stage* stage_;
	
	// �e�I�u�W�F�N�g�̐�
	static const size_t maxFloor_ = 169;

	int openOrCloseH_;
	Sprite* openOrClose_;
	bool isText_;

<<<<<<< HEAD:Scene1.h
	std::unique_ptr<Sprite> hintText_;
	int hintTextH_;
=======
	// ���C�g
	LightGroup* lightGroup_;

	// ���f��
	Model* sphereM_;
	Model* cubeM_;
	Model* floorM_;

	// �I�u�W�F�N�g
	Object3D* object_[3];
>>>>>>> master:Scene1.h

=======
>>>>>>> parent of fcb6802 (終了):GameScene.h
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

<<<<<<< HEAD:GameScene.h
private:
	// �����蔻�菈��
	void Collision();

	void WallCol();// -> �ǂƃv���C���[�̓����蔻��
	void WallSlide(Line2D& wall);// -> �ǂ��菈��

	void DoorCol();

};
=======
>>>>>>> master:Scene1.h
