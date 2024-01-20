/**
 * @file Zombie.h
 * @brief ゾンビのクラス
 * @author moomin
 * @date 2024/01/20
 */

#pragma once
#include "Object3D.h"

#include "CollisionManager3D.h"
#include "AABBCollider.h"
#include "RayCollider.h"

#include "NavMesh.h"
#include "Player.h"

#include <memory>

class Zombie {
private:
	// 状態
	enum class State {
		WAIT,
		CHASE,
	};

#pragma region メンバ変数
private:
	static CollisionManager3D* sColMgr_;
	static Model* sModel_;
	static Player* sPlayer_;
	static NavMesh* sNavMesh_;

	// オブジェクト3D
	std::unique_ptr<Object3D> object_ = nullptr;

	// コライダー
	std::unique_ptr<AABBCollider> headC_ = nullptr;// 頭
	std::unique_ptr<AABBCollider> bodyC_ = nullptr;// 体
	std::unique_ptr<RayCollider> legC_ = nullptr;// 接地

	Vector3 headOffset_ = { 0.0f, 1.75f, 0.0f };
	Vector3 headRadius_ = { 0.25f, 0.25f, 0.25f };
	Vector3 bodyOffset_ = { 0.0f, 0.75f, 0.0f };
	Vector3 bodyRadius_ = { 0.25f, 0.75f, 0.25f };

	// 状態
	State state_ = State::CHASE;

	// HP
	uint8_t hp_ = 3;

	// 生存フラグ
	bool isAlive_ = true;

	// 接地フラグ
	bool isGround_ = false;

	// 重力
	float gravity_ = 0.0f;
	const float velocityLimit_ = 1.0f;
	float velocity_ = 0.0f;// 速度
	float accel_ = 0.05f;// 加速度
	float JumpSpd_ = -0.5f;

	// ゾンビの向き
	float turnSpd_ = 8.0f;
	float angle_ = 0.0f;
	Vector2 forwardVec_ = { 0.0f, 1.0f };

	// ルート探索
	std::vector<Vector3> route_ = {};
	std::unique_ptr<Line3D> line_ = nullptr;
	float routeSearchInterval_ = 1.0f;// ルート探索のインターバル[s]
	uint64_t lastRouteSearchTime_ = 0;// 最後にルート探索した時間
	float moveSpd_ = 0.05f;
	float visualRecognitionDist_ = 40.0f;// 視認距離

	Vector3 knockBackVec_ = { 0.0f, 0.0f, 0.0f };
	float knockBackSpd_ = 0.0f;

	float hitReactionDuringTime_ = 0.2f;
	uint64_t hitTime_ = 0;

	bool isDebug_ = false;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Zombie() {}

	// デストラクタ
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

	// デバック
	void Debug(bool isDebug);

private:
	// 状態別処理
	static void (Zombie::* stateTable[]) ();
	void Wait();	// 待機
	void Chase();	// 追跡

	// ルート作成
	void CreateRoute();

	// 移動処理
	void Move();

	// 接地判定
	void GroundingJudgment();

	// 重力処理
	void Gravity();

	// 回転処理
	void Rotate();

	// 攻撃を受けた際の処理
	void Hit();

	// ジャンプ処理
	void Jump();
#pragma endregion

#pragma region セッター関数
public:
	static void SetCollisionManager(CollisionManager3D* inColMgr) { sColMgr_ = inColMgr; }
	static void SetModel(Model* inModel) { sModel_ = inModel; }
	static void SetPlayer(Player* inPlayer) { sPlayer_ = inPlayer; }
	static void SetNavMesh(NavMesh* inNavMesh) { sNavMesh_ = inNavMesh; }
#pragma endregion

#pragma region ゲッター関数
public:
	bool GetIsAlive() { return isAlive_; }
	const Vector3& GetPosition() { return object_->GetPosition(); }
#pragma endregion
};