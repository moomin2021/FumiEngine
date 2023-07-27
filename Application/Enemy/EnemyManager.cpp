#include "EnemyManager.h"
#include "Enemy0.h"

EnemyManager::EnemyManager() {
	// モデル読み込み
	mEnemy0_ = std::make_unique<Model>("sphere");
	mEnemy0Bullet_ = std::make_unique<Model>("sphere");

	Enemy0::SetBulletModel(mEnemy0Bullet_.get());
}

void EnemyManager::Update()
{
	// 敵更新処理
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
	// 敵描画処理
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
	// 敵の生成
	std::unique_ptr<Enemy0> newEnemy = std::make_unique<Enemy0>(mEnemy0_.get());
	newEnemy->Initialize(pos, scale);

	// エネミー配列に追加
	enemys_.emplace_back(std::move(newEnemy));
}

void EnemyManager::SetPlayer(Player* player)
{
	Enemy0::SetPlayer(player);
}