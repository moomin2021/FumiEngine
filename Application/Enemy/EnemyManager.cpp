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

	//for (size_t i = 0; i < enemys_.size(); i++) {
	//	enemys_[i]->Update();
	//}
}

void EnemyManager::Draw()
{
	// �G�`�揈��
	std::list<std::unique_ptr<BaseEnemy>>::iterator it;
	for (it = enemys_.begin(); it != enemys_.end(); it++) {
		(*it)->Draw();
	}

	//for (size_t i = 0; i < enemys_.size(); i++) {
	//	enemys_[i]->Draw();
	//}
}

EnemyManager::EnemyManager() {

}