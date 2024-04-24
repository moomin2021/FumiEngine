#include "EnemyFactory.h"

#include "Zombie.h"

void EnemyFactory::Initialize()
{
	colMgr_ = CollisionManager3D::GetInstance();

	// モデル
	zombie0M_ = std::make_unique<Model>("zombie0");
	zombie1M_ = std::make_unique<Model>("zombie1");
}

BaseEnemy* EnemyFactory::CreateZombie(const Vector3& inPos)
{
	Zombie* newEnemy = new Zombie();
	newEnemy->SetCollisionManager3D(colMgr_);
	newEnemy->SetNavMesh(pNavMesh_);
	newEnemy->SetPlayer(pPlayer_);
	newEnemy->SetModel(zombie0M_.get(), zombie1M_.get());

	newEnemy->Initialize(inPos);
	return newEnemy;
}