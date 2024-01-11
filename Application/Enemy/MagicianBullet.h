#pragma once
#include "Vector3.h"
#include "ParticleEmitter.h"
#include "CollisionManager.h"
#include "AABBCollider.h"

#include <memory>

class MagicianBullet
{
#pragma region メンバ変数
private:
	static CollisionManager* sColMgr_;
	static uint16_t sParticleHandle_;

	Vector3 pos_ = { 0.0f, 0.0f, 0.0f };// 座標
	Vector3 dir_ = { 0.0f, 0.0f, 0.0f };// 方向
	float speed_ = 0.1f;// 速度
	float radius_ = 0.25f;// 半径
	bool isAlive_ = true;// 生存フラグ

	// パーティクルエミッター
	std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;
	uint16_t particleLife_ = 60;// パーティクルの寿命(フレーム)
	uint16_t maxOneTimeGenerate_ = 10;// 一度に生成する最大数
	uint16_t minOneTimeGenerate_ = 5;// 一度に生成する最小数

	// コライダー
	std::unique_ptr<AABBCollider> collider_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化処理
	void Initialize(const Vector3& startPos, const Vector3& inDir);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	MagicianBullet() {}
	~MagicianBullet();

private:
	// パーティクルの生成
	void CreateParticle();
#pragma endregion

#pragma region セッター関数
public:
	// 衝突判定マネージャーを設定
	static void SetCollisionManager(CollisionManager* inColMgr) { sColMgr_ = inColMgr; }

	// パーティクルの画像の設定
	static void SetParticleHandle(uint16_t inHandle) { sParticleHandle_ = inHandle; }
#pragma endregion

#pragma region ゲッター関数
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};