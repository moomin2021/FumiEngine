#include "StageManager.h"
#include "CollisionAttribute.h"

StageManager::~StageManager()
{
	for (auto& it : stageObjColliders_) {
		colMgr_->RemoveCollider(it.get());
	}
}

void StageManager::Initialize()
{
	// �R���W�����}�l�[�W���[�C���X�^���X�擾
	colMgr_ = CollisionManager::GetInstance();
}

void StageManager::ObjUpdate()
{
	// �I�u�W�F�N�g�X�V����
	for (auto& it : stageObjects_) {
		it->Update();
	}
}

void StageManager::Draw()
{
	// �I�u�W�F�N�g�`��
	for (auto& it : stageObjects_) {
		it->Draw();
	}
}

void StageManager::AddModel(std::string fileName)
{
	// �������O�̃��f�������������烂�f����ǉ�
	if (stageModels_.count(fileName) == 0) {
		stageModels_.emplace(fileName, std::make_unique<Model>(fileName));
	}
}

void StageManager::AddObject3D(std::string modelName, const float3& position, const float3& rotation, const float3& scale)
{
	// ���f�������w�肵�Đ���
	stageObjects_.emplace_back(std::make_unique<Object3D>(stageModels_[modelName].get()));

	// �f�[�^��ݒ�
	stageObjects_.back()->SetPosition(position);
	stageObjects_.back()->SetRotation(rotation);
	stageObjects_.back()->SetScale(scale);

	// �R���C�_�[�𐶐�
	stageObjColliders_.emplace_back(std::make_unique<MeshCollider>(stageObjects_.back().get()));
	stageObjColliders_.back()->SetAttribute(COL_STAGE_OBJ);
	stageObjColliders_.back()->LinkObject3D(stageObjects_.back().get());

	// �R���C�_�[��o�^
	colMgr_->AddCollider(stageObjColliders_.back().get());
}