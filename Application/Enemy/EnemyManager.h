#pragma once
#include "BaseEnemy.h"

#include <memory>
#include <list>

class EnemyManager {
#pragma region �����o�ϐ�
	// �G
	std::list<std::unique_ptr<BaseEnemy>> enemys_;
#pragma endregion

#pragma region �����o�֐�
public:
	// �C���X�^���X�擾
	static EnemyManager* GetInstance();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	/// <summary>
	/// �G�ǉ�
	/// </summary>
	/// <param name="enemy"> �G </param>
	void AddEnemy(BaseEnemy* enemy) { enemys_.emplace_back(enemy); }

private:
	// �R���X�g���N�^
	EnemyManager();
#pragma endregion

#pragma region ����֐�
	EnemyManager(const EnemyManager&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	EnemyManager& operator = (const EnemyManager&) = delete;// �R�s�[������Z�q�֎~
#pragma endregion
};