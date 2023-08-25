#pragma once
#include "Object3D.h"
#include "MeshCollider.h"
#include "CollisionManager.h"

#include <memory>
#include <deque>

class StageManager
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	CollisionManager* colMgr_ = nullptr;

	// �X�e�[�W�̊e���f��
	std::map<std::string, std::unique_ptr<Model>> stageModels_;

	// �X�e�[�W�̊e�I�u�W�F�N�g
	std::deque<std::unique_ptr<Object3D>> stageObjects_;

	// �X�e�[�W�̊e�I�u�W�F�N�g�̃R���C�_�[
	std::deque<std::unique_ptr<MeshCollider>> stageObjColliders_;
#pragma endregion

#pragma region �����o�֐�
public:
	// �f�X�g���N�^
	~StageManager();

	// ����������
	void Initialize();

	// �X�V����
	void ObjUpdate();

	// �Փˎ�����
	void OnCollision();

	// �`�揈��
	void Draw();

	// ���f����ǉ�
	void AddModel(std::string fileName);

	// �I�u�W�F�N�g3D�̒ǉ�
	void AddObject3D(std::string modelName, const float3& position, const float3& rotation, const float3& scale);
#pragma endregion
};