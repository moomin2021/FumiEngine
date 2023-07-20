#pragma once
#include "BaseEnemy.h"

#include <memory>
#include <vector>
#include <list>

class EnemyManager {
#pragma region �����o�ϐ�
	// �G�l�~�[�z��
	std::list<std::unique_ptr<BaseEnemy>> enemys_;

	// �G���f��
	std::unique_ptr<Model> mEnemy0_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	EnemyManager();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �G�l�~�[�𐶐��ǉ�
	void CreateAddEnemy0(const float3& pos, const float3& scale);
#pragma endregion

#pragma region �Z�b�^�[�֐�
	
#pragma endregion

#pragma region �Q�b�^�[�֐�

#pragma endregion

#pragma region ����֐�
	EnemyManager(const EnemyManager&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	EnemyManager& operator = (const EnemyManager&) = delete;// �R�s�[������Z�q�֎~
#pragma endregion
};