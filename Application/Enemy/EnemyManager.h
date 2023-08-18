#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Boss0.h"

#include <memory>

class EnemyManager
{
#pragma region メンバ変数
private:
	// インスタンス
	CollisionManager* colMgr_ = nullptr;

	// モデル
	std::unique_ptr<Model> mBossGenerator_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3D> oBossGenerator_ = nullptr;

	// コライダー
	std::unique_ptr<SphereCollider> colBossGenerator_ = nullptr;

	// ボス
	std::unique_ptr<Boss0> boss_ = nullptr;
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
	void ObjUpdate();

	// 衝突時処理
	void OnCollision();

	// ボス召喚
	void SummonBoss();

private:
#pragma endregion

#pragma region セッター関数
public:
	void SetBossGenerator(const float3& pos);
#pragma endregion
};