#pragma once
#include "Model.h"
#include "Object3D.h"
#include "CollisionManager.h"
#include "RayCollider.h"
#include "SphereCollider.h"

#include <memory>

class Zombie
{
private:
	enum class State {
		NONE,
		PATROL,
		CHASE,
	};

#pragma region メンバ変数
private:
	// 衝突判定管理クラス
	static CollisionManager* sColMgr_;

	// モデル
	static Model* sModel_;

	// オブジェクト3D
	std::unique_ptr<Object3D> object_ = nullptr;

	// コライダー
	std::unique_ptr<RayCollider> cRay_ = nullptr;// 接地判定を取るコライダー
	std::unique_ptr<SphereCollider> cSphere_ = nullptr;// ステージObjと衝突判定をする

	// ステート
	State state_ = State::NONE;

	// 生存フラグ
	bool isAlive_ = true;

	// 接地している
	bool isGround_ = false;

	// 重力
	Vector3 gravity_ = { 0.0f, -1.0f, 0.0f };
	const float velocityLimit_ = 1.0f;
	float velocity_ = 0.0f;// 速度
	float accel_ = 0.05f;// 加速度

#pragma endregion

#pragma region メンバ関数
public:
	Zombie() {}
	~Zombie();

	// 初期化処理
	void Initialize(const Vector3& inPos);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

private:
	// 状態別処理
	static void (Zombie::* stateTable[]) ();
	void Patrol();		// 見回り状態
	void Chase();		// 追跡状態

	void GroundingJudgment();// 接地判定
	void Gravity();// 重力処理
#pragma endregion

#pragma region セッター関数
public:
	static void SetCollisionManager(CollisionManager* inColMgr) { sColMgr_ = inColMgr; }
	static void SetModel(Model* inModel) { sModel_ = inModel; }
#pragma endregion

#pragma region ゲッター関数
public:
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};