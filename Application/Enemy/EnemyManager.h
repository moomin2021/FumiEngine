#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Boss0.h"
#include "Enemy0.h"
#include "Zombie.h"
#include "ParticleEmitter.h"
#include "NavMesh.h"
#include "EnemyGenerator.h"
#include "EnemyCore.h"

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

	// パーティクルエミッター
	std::deque<std::unique_ptr<ParticleEmitter>> particles_;

	// パーティクル画像のハンドル
	uint16_t hParticle_ = 0;

	// オブジェクト
	std::unique_ptr<Object3D> oBossGenerator_ = nullptr;

	// コライダー
	std::unique_ptr<SphereCollider> colBossGenerator_ = nullptr;

	// ボス
	std::unique_ptr<Boss0> boss_ = nullptr;

	// エネミー
	std::deque<std::unique_ptr<Enemy0>> enemys_;
	std::deque<std::unique_ptr<Zombie>> zombies_ = {};

	// ナビメッシュ
	std::unique_ptr<NavMesh> navMesh_ = nullptr;

	// コア
	std::unique_ptr<EnemyCore> enemyCore_ = nullptr;

	// 敵の生成器
	std::unique_ptr<EnemyGenerator> enemyGenerator_ = nullptr;
	std::vector<Vector3> cellsCenter_ = {};

	uint16_t enemyDeathCounter_ = 0;
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

	// ボス召喚
	void SummonBoss();

	// エネミーを生成追加
	void CreateAddEnemy0(const Vector3& pos);
	void DeleteEnemy0() { enemys_.clear(); }

	void CheckSceneChange();

	// パーティクル生成
	void AddParticle(const Vector3& pos);

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
