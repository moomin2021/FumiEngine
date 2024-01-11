#pragma once
#include "Model.h"
#include "Object3D.h"
#include "PointLight.h"
#include "LightGroup.h"
#include "CollisionManager.h"
#include "SphereCollider.h"

#include "Player.h"

#include <memory>

class EnemyCore
{
#pragma region メンバ変数
private:
	static LightGroup* sLightGroup_;
	static CollisionManager* sColMgr_;
	static Player* sPlayer_;

	// モデル
	static Model* sCoreM_;
	static Model* sCoreFrameM_;
	static Model* sCoreStandM_;

	// オブジェクト
	std::unique_ptr<Object3D> coreO_ = nullptr;
	std::unique_ptr<Object3D> coreFrameO_ = nullptr;
	std::unique_ptr<Object3D> coreStandO_ = nullptr;

	// コライダー
	std::unique_ptr<SphereCollider> collider_ = nullptr;

	// オブジェクトのオフセット
	Vector3 offset0_ = { 0.0f, 0.0f, 0.0f };
	Vector3 offset1_ = { 0.0f, 1.5f, 0.0f };

	// HP
	const int32_t maxHP_ = 3;
	int32_t hp_ = maxHP_;

	// 生存フラグ
	bool isAlive_ = true;

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;
	Vector3 lightPosition_ = { 0.0f, 0.0f, 0.0f };
	float lightIntensity_ = 1.0f;
	float lightRadius_ = 5.0f;
	float maxLightIntensity_ = 10.0f;
	float minLightIntensity_ = 1.0f;
	float subLightIntensity_ = 0.5f;

	float dLightDecay_ = 1.0f;

	// 沸かせる
	bool isSpawn_ = false;
	Vector3 spawnPos_ = { 0.0f, 0.0f, 0.0f };
	uint64_t lastSpawnTime_ = 0;
	float interval_ = 5.0f;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化処理
	void Initialize(const Vector3& inPos);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突判定時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	// デバック
	void Debug();

	EnemyCore() {}
	~EnemyCore();
#pragma endregion

#pragma region セッター関数
	static void SetModel(Model* inCoreM, Model* inCoreFrameM, Model* inCoreStandM)
	{
		sCoreM_ = inCoreM;
		sCoreFrameM_ = inCoreFrameM;
		sCoreStandM_ = inCoreStandM;
	}

	static void SetLightGroup(LightGroup* inLightGroup) { sLightGroup_ = inLightGroup; }
	static void SetCollisionManager(CollisionManager* inColMgr) { sColMgr_ = inColMgr; }
	static void SetPlayer(Player* inPlayer) { sPlayer_ = inPlayer; }
#pragma endregion

#pragma region ゲッター関数
	bool GetIsAlive() { return isAlive_; }
	const Vector3& GetPosition() { return coreO_->GetPosition(); }
	bool GetIsSpawn() { return isSpawn_; }
	const Vector3& GetSpawnPos() { return spawnPos_; }
#pragma endregion
};