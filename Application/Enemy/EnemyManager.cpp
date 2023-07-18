#include "EnemyManager.h"

EnemyManager* EnemyManager::GetInstance()
{
	// �C���X�^���X����
	static EnemyManager inst;

	// �C���X�^���X��Ԃ�
	return &inst;
}

void EnemyManager::Update()
{
	// �G�X�V����
	std::list<std::unique_ptr<BaseEnemy>>::iterator it;
	for (it = enemys_.begin(); it != enemys_.end(); it++) {
		(*it)->Update();
	}
}

void EnemyManager::Draw()
{
	// �G�`�揈��
	std::list<std::unique_ptr<BaseEnemy>>::iterator it;
	for (it = enemys_.begin(); it != enemys_.end(); it++) {
		(*it)->Draw();
	}
}

EnemyManager::EnemyManager() {

}