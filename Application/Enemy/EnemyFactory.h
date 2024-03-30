#pragma once
#include "BaseEnemy.h"
#include "NavMesh.h"
#include "Player.h"

#include <memory>

class EnemyFactory
{
#pragma region メンバ変数
	CollisionManager3D* colMgr_ = nullptr;

	// モデル
	std::unique_ptr<Model> zombie0M_ = nullptr;
	std::unique_ptr<Model> zombie1M_ = nullptr;

	NavMesh* pNavMesh_ = nullptr;
	Player* pPlayer_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	EnemyFactory(NavMesh* inNavMesh, Player* inPlayer) : pNavMesh_(inNavMesh), pPlayer_(inPlayer) {}
	~EnemyFactory() {}

	void Initialize();

	BaseEnemy* CreateZombie();
#pragma endregion
};