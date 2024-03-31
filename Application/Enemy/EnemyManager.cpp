#include "EnemyManager.h"

#include "PipelineManager.h"
#include "Zombie.h"

void EnemyManager::Initialize()
{
	// シングルトンインスタンスの取得
	colMgr_ = CollisionManager3D::GetInstance();
	key_ = Key::GetInstance();

	// エネミー生成器
	enemyFactory_ = std::make_unique<EnemyFactory>(pNavMesh_, pPlayer_);
	enemyFactory_->Initialize();
}

void EnemyManager::Update()
{
	for (auto it = enemys_.begin(); it != enemys_.end();)
	{
		(*it)->Update();

		if ((*it)->GetIsAlive() == false)
		{
			(*it)->Finalize();
			it = enemys_.erase(it);
		}

		else ++it;
	}

	if (key_->TriggerKey(DIK_C))
	{
		CreateEnemy();
	}
}

void EnemyManager::Draw()
{
	PipelineManager::PreDraw("Object3D");

	for (auto& it : enemys_) it->Draw();
}

void EnemyManager::Collision()
{
	colMgr_->CheckAllCollision();

	for (auto& it : enemys_) it->Collision();
}

void EnemyManager::MatUpdate()
{
	for (auto& it : enemys_) it->MatUpdate();
}

void EnemyManager::Finalize()
{
	for (auto& it : enemys_) it->Finalize();
	enemys_.clear();
}

void EnemyManager::CreateEnemy()
{
	enemys_.emplace_back(enemyFactory_->CreateZombie());
}