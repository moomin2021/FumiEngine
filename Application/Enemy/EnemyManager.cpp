#include "EnemyManager.h"

#include "PipelineManager.h"
#include "Zombie.h"

void EnemyManager::Initialize()
{
	// シングルトンインスタンスの取得
	colMgr_ = CollisionManager3D::GetInstance();
	key_ = Key::GetInstance();

	// エネミー生成器
	enemyFactory_ = std::make_unique<EnemyFactory>();
	enemyFactory_->Initialize();

	// モデル
	coreM_ = std::make_unique<Model>("core");
	coreFrameM_ = std::make_unique<Model>("coreFrame");
	coreStandM_ = std::make_unique<Model>("coreStand");
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

	for (auto it = cores_.begin(); it != cores_.end();)
	{
		(*it)->Update();

		if ((*it)->GetIsAlive() == false)
		{
			(*it)->Finalize();
			it = cores_.erase(it);
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
	for (auto& it : cores_) it->Draw();
}

void EnemyManager::Collision()
{
	colMgr_->CheckAllCollision();

	for (auto& it : enemys_) it->Collision();
	for (auto& it : cores_) it->Collision();
}

void EnemyManager::MatUpdate()
{
	for (auto& it : enemys_) it->MatUpdate();
	for (auto& it : cores_) it->MatUpdate();
}

void EnemyManager::Finalize()
{
	for (auto& it : enemys_) it->Finalize();
	enemys_.clear();
	for (auto& it : cores_) it->Finalize();
	cores_.clear();
}

void EnemyManager::Debug(bool isDebug)
{
	for (auto& it : enemys_) it->Debug(isDebug);
}

void EnemyManager::CreateEnemy()
{
	enemys_.emplace_back(enemyFactory_->CreateZombie());
}

void EnemyManager::CreateCore(const Vector3& inPos)
{
	std::unique_ptr<EnemyCore> newCore = std::make_unique<EnemyCore>();
	newCore->SetLightGroup(pLightGroup_);
	newCore->SetCollisionManager(colMgr_);
	newCore->SetPlayer(pPlayer_);
	newCore->SetModel(coreM_.get(), coreFrameM_.get(), coreStandM_.get());
	newCore->Initialize(inPos);

	cores_.emplace_back(std::move(newCore));
}