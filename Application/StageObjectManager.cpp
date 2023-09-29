#include "StageObjectManager.h"
#include "PipelineManager.h"

StageObjectManager::StageObjectManager() {}

StageObjectManager::~StageObjectManager()
{
}

void StageObjectManager::Initialize()
{
}

void StageObjectManager::Update()
{
}

void StageObjectManager::Draw()
{
	// �`��ݒ���I�u�W�F�N�g3D�p��
	PipelineManager::PreDraw("Object3D");

	// �I�u�W�F�N�g�`�揈��
	for (auto& object : objects_) object->Draw();
}

void StageObjectManager::OnCollision()
{
}

void StageObjectManager::MatUpdate()
{
	// �I�u�W�F�N�g�s��X�V
	for (auto& object : objects_) object->MatUpdate();
}