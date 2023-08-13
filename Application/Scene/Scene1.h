#pragma once
#include "BaseScene.h"
#include "LightGroup.h"
#include "EnemyManager.h"
#include "Player.h"
#include "MeshCollider.h"
#include "ItemManager.h"

class Scene1 : public BaseScene {
#pragma region �\����
	// ���x���f�[�^
	struct LevelData {
		// �I�u�W�F�N�g1���̃f�[�^
		struct ObjectData {
			// �t�@�C����
			int isActive;
			std::string fileName;
			std::string className;
			float3 translation;
			float3 rotation;
			float3 scaling;
		};

		std::vector<ObjectData> objects;
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// ���C�g
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// �G�l�~�[�}�l�[�W���[
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

	// �v���C���[
	std::unique_ptr<Player> player_ = nullptr;

	// �X�e�[�W�I�u�W�F�N�g
	std::map<std::string, std::unique_ptr<Model>> stageModels_;// ���f���A�z�z��
	std::list<std::unique_ptr<Object3D>> stageObjects_;// �I�u�W�F�N�g�z��
	std::list<std::unique_ptr<MeshCollider>> stageObjCollider_;
#pragma endregion

#pragma region �����o�֐�
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

private:
	// �X�e�[�W��ǂݍ���
	void LoadStage(std::string fileName);
#pragma endregion
};