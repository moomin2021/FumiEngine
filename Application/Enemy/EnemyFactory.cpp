#include "EnemyFactory.h"

#include "Zombie.h"

void EnemyFactory::Initialize()
{
	colMgr_ = CollisionManager3D::GetInstance();

	// モデル
	zombie0M_ = std::make_unique<Model>("zombie0");
	zombie1M_ = std::make_unique<Model>("zombie1");
}

BaseEnemy* EnemyFactory::CreateZombie()
{
	Zombie* newEnemy = new Zombie();
	newEnemy->SetCollisionManager3D(colMgr_);
	newEnemy->SetNavMesh(pNavMesh_);
	newEnemy->SetPlayer(pPlayer_);
	newEnemy->SetModel(zombie0M_.get(), zombie1M_.get());

	newEnemy->Initialize(Vector3(0.0f, 0.0f, 5.0f));
	return newEnemy;
}