#include "BossGenerator.h"
#include "CollisionManager.h"

BossGenerator::BossGenerator() {
#pragma region �W�F�l���[�^�[
	// ���f��
	mGen_ = std::make_unique<Model>();// ����

	// �I�u�W�F�N�g
	oGen_ = std::make_unique<Object3D>(mGen_.get());// ����

	// �R���C�_�[
	cGen_ = std::make_unique<SphereCollider>();// ����
	CollisionManager::GetInstance()->AddCollider(cGen_.get());// �R���C�_�[��ǉ�
#pragma endregion

	// �{�X
	boss_ = std::make_unique<Boss>();
}

void BossGenerator::Update()
{
	// �I�u�W�F�N�g�X�V����
	oGen_->Update();
}

void BossGenerator::Draw()
{
	// �I�u�W�F�N�g�`�揈��
	oGen_->Draw();
}