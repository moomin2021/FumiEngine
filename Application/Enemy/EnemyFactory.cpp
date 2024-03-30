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
	BaseEnemy* newEnemy = new Zombie(colMgr_, pNavMesh_, pPlayer_, zombie0M_.get(), zombie1M_.get());
	newEnemy->Initialize(Vector3(0.0f, 7.0f, 0.0f));
	return newEnemy;
}