#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Boss0.h"
#include "Enemy0.h"

#include <memory>

class EnemyManager
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	CollisionManager* colMgr_ = nullptr;

	// ���f��
	std::unique_ptr<Model> mBossGenerator_ = nullptr;
	std::unique_ptr<Model> mEnemy0_ = nullptr;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> oBossGenerator_ = nullptr;

	// �R���C�_�[
	std::unique_ptr<SphereCollider> colBossGenerator_ = nullptr;

	// �{�X
	std::unique_ptr<Boss0> boss_ = nullptr;

	// �G�l�~�[
	std::deque<std::unique_ptr<Enemy0>> enemys_;
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

	// �G�l�~�[�𐶐��ǉ�
	void CreateAddEnemy0(const Vector3& pos, const Vector3& scale);

	void CheckSceneChange();

private:
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	void SetBossGenerator(const Vector3& pos);

	// �v���C���[��ݒ�
	void SetPlayer(Player* player);
#pragma endregion
};