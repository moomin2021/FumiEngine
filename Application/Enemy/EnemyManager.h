/**
 * @file EnemyManager.h
 * @brief エネミー関連を管理するクラス
 * @author moomin
 * @date 2024/04/17
 */

 // このクラスのインスタンスは初期化する前に各データのポインタを取得しておく必要がある
 // 以下取得初期化前に取得しておくべきデータ一覧
 // LightGroup
 // NavMesh
 // Player

#pragma once
#include "BasicTask.h"
#include "BaseEnemy.h"
#include "EnemyFactory.h"
#include "EnemyCore.h"
#include "NavMesh.h"

#include "Player.h"
#include "LightGroup.h"
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

	LightGroup* pLightGroup_ = nullptr;
	NavMesh* pNavMesh_ = nullptr;
	Player* pPlayer_ = nullptr;

	std::unique_ptr<Model> coreM_ = nullptr;
	std::unique_ptr<Model> coreFrameM_ = nullptr;
	std::unique_ptr<Model> coreStandM_ = nullptr;

	// エネミー
	std::deque<std::unique_ptr<BaseEnemy>> enemys_ = {};
	std::deque<std::unique_ptr<EnemyCore>> cores_ = {};

	// エネミー生成器
	std::unique_ptr<EnemyFactory> enemyFactory_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	EnemyManager() {}
	~EnemyManager() {}

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Collision() override;
	void MatUpdate() override;
	void Finalize() override;
	void Debug(bool isDebug);

	void CreateEnemy();
	void CreateCore(const Vector3& inPos);
#pragma endregion

#pragma region セッター関数
public:
	void SetLightGroup(LightGroup* inLightGroup) { pLightGroup_ = inLightGroup; }
	void SetNavMesh(NavMesh* inNavMesh) { pNavMesh_ = inNavMesh, enemyFactory_->SetNavMesh(inNavMesh); }
	void SetPlayer(Player* inPlayer) { pPlayer_ = inPlayer, enemyFactory_->SetPlayer(inPlayer); }
#pragma endregion
};