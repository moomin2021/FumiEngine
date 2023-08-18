#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Boss0.h"

#include <memory>

class EnemyManager
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	CollisionManager* colMgr_ = nullptr;

	// ���f��
	std::unique_ptr<Model> mBossGenerator_ = nullptr;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> oBossGenerator_ = nullptr;

	// �R���C�_�[
	std::unique_ptr<SphereCollider> colBossGenerator_ = nullptr;

	// �{�X
	std::unique_ptr<Boss0> boss_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	EnemyManager();

	// �f�X�g���N�^
	~EnemyManager();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �I�u�W�F�N�g�X�V
	void ObjUpdate();

	// �Փˎ�����
	void OnCollision();

	// �{�X����
	void SummonBoss();

private:
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	void SetBossGenerator(const float3& pos);
#pragma endregion
};