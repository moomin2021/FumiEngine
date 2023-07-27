#include "EnemyManager.h"
#include "Enemy0.h"

EnemyManager::EnemyManager() {
	// ���f���ǂݍ���
	mEnemy0_ = std::make_unique<Model>("sphere");
	mEnemy0Bullet_ = std::make_unique<Model>("sphere");

	Enemy0::SetBulletModel(mEnemy0Bullet_.get());
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

void EnemyManager::CreateAddEnemy0(const float3& pos, const float3& scale)
{
	// �G�̐���
	std::unique_ptr<Enemy0> newEnemy = std::make_unique<Enemy0>(mEnemy0_.get());
	newEnemy->Initialize(pos, scale);

	// �G�l�~�[�z��ɒǉ�
	enemys_.emplace_back(std::move(newEnemy));
}

void EnemyManager::SetPlayer(Player* player)
{
	Enemy0::SetPlayer(player);
}