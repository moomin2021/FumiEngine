#pragma once
#include "StageObjectManager.h"
#include "Model.h"
#include "Vector3.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

class LoadStage {
	// ���x���f�[�^
	struct LevelData {
		// �I�u�W�F�N�g1���̃f�[�^
		struct ObjectData {
			// �t�@�C����
			int isActive;
			std::string fileName;
			std::string className;
			Vector3 translation;
			Vector3 rotation;
			Vector3 scaling;
		};

		std::vector<ObjectData> objects;
	};

#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	StageObjectManager* stageObjMgr_ = nullptr;

	// �t�@�C�����ƕR�Â��ă��f����ۑ�
	std::map<std::string, std::unique_ptr<Model>> models_ = {};
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	LoadStage();

	// �f�X�g���N�^
	~LoadStage();

	// �X�e�[�W�ǂݍ���
	void Load(std::string fileName);
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// �X�e�[�W�I�u�W�F�N�g�Ǘ��p�N���X�̃C���X�^���X���擾
	void SetStageObjectManager(StageObjectManager* inst) { stageObjMgr_ = inst; }
#pragma endregion
};