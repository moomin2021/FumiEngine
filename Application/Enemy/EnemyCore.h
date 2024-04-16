/**
 * @file EnemyCore.h
 * @brief プレイヤーが壊す必要のあるコア
 * @author moomin
 * @date 2024/04/17
 */

// このクラスのインスタンスは初期化する前に各データのポインタを取得しておく必要がある
// 以下取得初期化前に取得しておくべきデータ一覧
// LightGroup
// CollisionManager3D
// Player
// Model

#pragma once
#include "Model.h"
#include "Object3D.h"

#include "LightGroup.h"
#include "Player.h"

#include "CollisionManager3D.h"
#include "AABBCollider.h"

#include <memory>

class EnemyCore {
#pragma region メンバ変数
private:
	LightGroup* pLightGroup_ = nullptr;
	CollisionManager3D* pColMgr_ = nullptr;
	Player* pPlayer_ = nullptr;

	Model* pCoreM_ = nullptr;
	Model* pCoreFrameM_ = nullptr;
	Model* pCoreStandM_ = nullptr;

	std::unique_ptr<Object3D> coreO_ = nullptr;
	std::unique_ptr<Object3D> coreFrameO_ = nullptr;
	std::unique_ptr<Object3D> coreStandO_ = nullptr;

	std::unique_ptr<AABBCollider> coreC_ = nullptr;

	bool isAlive_ = true;
	const int32_t maxHP_ = 3;
	int32_t hp_ = maxHP_;

	// オブジェクトのオフセット
	Vector3 offset0_ = { 0.0f, 0.0f, 0.0f };
	Vector3 offset1_ = { 0.0f, 1.5f, 0.0f };

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;
	Vector3 lightPosition_ = { 0.0f, 0.0f, 0.0f };
	float lightIntensity_ = 1.0f;
	float lightRadius_ = 5.0f;
	float maxLightIntensity_ = 10.0f;
	float minLightIntensity_ = 1.0f;
	float subLightIntensity_ = 0.5f;

	float dLightDecay_ = 1.0f;
#pragma endregion

#pragma region メンバ関数
public:
	void Initialize(const Vector3& inPos);
	void Update();
	void Draw();
	void Collision();
	void MatUpdate();
	void Finalize();

	EnemyCore() {}
	~EnemyCore() {}

private:
	void Hit();
#pragma endregion

#pragma region セッター関数
public:
	void SetLightGroup(LightGroup* inLightGroup) { pLightGroup_ = inLightGroup; }
	void SetCollisionManager(CollisionManager3D* inColMgr) { pColMgr_ = inColMgr; }
	void SetPlayer(Player* inPlayer) { pPlayer_ = inPlayer; }
	void SetModel(Model* inCoreM, Model* inCoreFrameM, Model* inCoreStandM);
#pragma endregion

#pragma region ゲッター関数
public:
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};