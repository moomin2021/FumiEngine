#pragma once
#include "Model.h"
#include "Object3D.h"
#include "MeshCollider.h"
#include "CollisionManager.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <forward_list>

class Stage
{
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
	// �Փ˔���Ǘ��N���X�C���X�^���X
	CollisionManager* colMgr_ = nullptr;

	// ���f���ۑ��p�A�z�z��
	std::unordered_map<std::string, std::unique_ptr<Model>> models_ = {};

	// �I�u�W�F�N�g
	std::forward_list<std::unique_ptr<Object3D>> objects_ = {};

	// �R���C�_�[
	std::forward_list<std::unique_ptr<MeshCollider>> colliders_ = {};
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Stage();

	// �f�X�g���N�^
	~Stage();

	// ����������
	void Initialize();

	// �s��X�V����
	void MatUpdate();

	// �`��X�V����
	void Draw();

	// �X�e�[�W���ǂݍ���
	void Load(std::string fileName);
#pragma endregion
};