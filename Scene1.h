#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Sprite.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
#include <vector>
#include <memory>

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_;

	// �J����
	std::unique_ptr<Camera> camera_;

	// ���C�g
	LightGroup* lightGroup_;

	// ���f��
	Model* mCube_;

	// �I�u�W�F�N�g
	std::vector<std::unique_ptr<Object3D>> oCube_;

	// �e�N�X�`���n���h��
	uint16_t haeHandle_;

	// �X�v���C�g
	std::unique_ptr<Sprite> sHae_;

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

