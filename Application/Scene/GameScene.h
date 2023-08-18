#pragma once
#include "BaseScene.h"
#include "LightGroup.h"
#include "StageManager.h"
#include "EnemyManager.h"
#include "Player.h"

#include <memory>

class GameScene : public BaseScene
{
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

	// �X�e�[�W�}�l�[�W���[
	std::unique_ptr<StageManager> stageMgr_ = nullptr;

	// �G�l�~�[�}�l�[�W���[
	std::unique_ptr<EnemyManager> enemyMgr_ = nullptr;

	// �v���C���[
	std::unique_ptr<Player> player_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
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
	// �I�u�W�F�N�g�X�V����
	void ObjUpdate();

	// �Փˎ�����
	void OnCollision();

	// �X�e�[�W��ǂݍ���
	void LoadStage(std::string fileName);
#pragma endregion
};