#pragma once
#include "BasicTask.h"
#include "BaseEnemy.h"
#include "EnemyFactory.h"
#include "NavMesh.h"

#include "Player.h"
#include "CollisionManager3D.h"
#include "Key.h"

#include <memory>
#include <deque>

class EnemyManager : public BasicTask
{
#pragma region メンバ変数
private:
	// シングルトンインスタンス
	CollisionManager3D* colMgr_ = nullptr;
	Key* key_ = nullptr;

	NavMesh* pNavMesh_ = nullptr;
	Player* pPlayer_ = nullptr;

	// エネミー
	std::deque<std::unique_ptr<BaseEnemy>> enemys_ = {};

	// エネミー生成器
	std::unique_ptr<EnemyFactory> enemyFactory_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	EnemyManager(NavMesh* inNavMesh, Player* inPlayer) : pNavMesh_(inNavMesh), pPlayer_(inPlayer) {}
	~EnemyManager() {}

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Collision() override;
	void MatUpdate() override;
	void Finalize() override;

	void CreateEnemy();
#pragma endregion
};