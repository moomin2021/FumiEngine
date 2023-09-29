#pragma once
#include "Model.h"
#include "Object3D.h"
#include "MeshCollider.h"
#include "CollisionManager.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <forward_list>

class StageObjectManager
{
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
	StageObjectManager();

	// �f�X�g���N�^
	~StageObjectManager();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ��X�V����
	void OnCollision();

	// �s��X�V����
	void MatUpdate();
#pragma endregion
};