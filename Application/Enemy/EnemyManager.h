#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Zombie.h"
#include "ParticleEmitter.h"
#include "NavMesh.h"
#include "EnemyGenerator.h"
#include "EnemyCore.h"
#include "DeltaTime.h"
#include "ResultData.h"

#include <memory>

class EnemyManager
{
#pragma region メンバ変数
private:
	// インスタンス
	CollisionManager* colMgr_ = nullptr;

	// モデル
	std::unique_ptr<Model> mBossGenerator_ = nullptr;
	std::unique_ptr<Model> mEnemy0_ = nullptr;
	std::unique_ptr<Model> mZombie_ = nullptr;
	std::unique_ptr<Model> coreM_ = nullptr;
	std::unique_ptr<Model> coreFrameM_ = nullptr;
	std::unique_ptr<Model> coreStandM_ = nullptr;

	// エネミー
	std::deque<std::unique_ptr<Zombie>> zombies_ = {};

	// ナビメッシュ
	std::unique_ptr<NavMesh> navMesh_ = nullptr;

	// コア
	std::deque<std::unique_ptr<EnemyCore>> enemyCores_ = {};

	// 敵の生成器
	std::deque<EnemyGenerator> enemyGenerators_ = {};
	std::vector<Vector3> cellsCenter_ = {};

	uint16_t enemyDeathCounter_ = 0;

	DeltaTime deltaTime_ = {};
	float elapsedTime_ = 0.0f;
	uint16_t killEnemy_ = 0;
	uint16_t breakCore_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	EnemyManager();

	// デストラクタ
	~EnemyManager();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// オブジェクト更新
	void MatUpdate();

	// 衝突時処理
	void OnCollision();

	// エネミーを生成追加
	void CreateAddEnemy0(const Vector3& pos);

	void AddCore(const Vector3& inPos);

	void CheckSceneChange();

	// デバック
	void Debug();

private:
#pragma endregion

#pragma region セッター関数
public:
	void SetBossGenerator(const Vector3& pos);

	// プレイヤーを設定
	void SetPlayer(Player* player);
#pragma endregion
};
