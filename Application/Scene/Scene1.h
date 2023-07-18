#pragma once
#include "BaseScene.h"
#include "LightGroup.h"
#include "LoadStage.h"
#include "Player.h"

#include <vector>

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// ���C�g
	std::unique_ptr<LightGroup> lightGroup_;
	std::unique_ptr<DirectionalLight> dirLight_;

	// ���f��
	std::unique_ptr<Model> mFloor_;
	std::unique_ptr<Model> mCube_;
	std::unique_ptr<Model> mSphere_;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> oFloor_;
	std::unique_ptr<Object3D> oSphere_;
	std::vector<std::unique_ptr<Object3D>> oCube_;

	// �e�N�X�`���n���h��
	uint16_t haeHandle_;
	// �X�e�[�W�ǂݍ��ݗp�N���X
	std::unique_ptr<LoadStage> loadStage_;

	// �v���C���[
	std::unique_ptr<Player> player_;

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

	void Collision();
};

