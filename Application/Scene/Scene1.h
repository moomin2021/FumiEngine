#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "LightGroup.h"
#include "LoadStage.h"
#include "Player.h"

#include <memory>

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �J����
	std::unique_ptr<Camera> camera_;

	// ���C�g
	std::unique_ptr<LightGroup> lightGroup_;
	std::unique_ptr<DirectionalLight> dirLight_;

	// �X�e�[�W�ǂݍ��ݗp�N���X
	std::unique_ptr<LoadStage> loadStage_;

	// �v���C���[
	std::unique_ptr<Player> player_;

	std::unique_ptr<Model> mCube_;
	std::unique_ptr<Object3D> oCube_;

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

