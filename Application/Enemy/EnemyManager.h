#pragma once
#include "BaseEnemy.h"
#include "Player.h"

#include <memory>
#include <vector>
#include <list>
#include <vector>

class EnemyManager {
#pragma region �����o�ϐ�
	// �G�l�~�[�z��
	std::list<std::unique_ptr<BaseEnemy>> enemys_;
	static std::vector<std::unique_ptr<Bullet>> bullets_;

	// �G���f��
	std::unique_ptr<Model> mEnemy0_ = nullptr;
	static std::unique_ptr<Model> mEnemy0Bullet_;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	EnemyManager();

	// �f�X�g���N�^
	~EnemyManager();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �G�l�~�[�𐶐��ǉ�
	void CreateAddEnemy0(const float3& pos, const float3& scale);

	// �e��ǉ�
	static void AddBullet(BulletType type, const float3& iniPos, const Vector3& moveVec);
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// �v���C���[��ݒ�
	void SetPlayer(Player* player);
#pragma endregion

#pragma region �Q�b�^�[�֐�

#pragma endregion

#pragma region ����֐�
	EnemyManager(const EnemyManager&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	EnemyManager& operator = (const EnemyManager&) = delete;// �R�s�[������Z�q�֎~
#pragma endregion
};