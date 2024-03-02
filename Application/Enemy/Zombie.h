/**
 * @file Zombie.h
 * @brief ゾンビのクラス
 * @author moomin
 * @date 2024/01/20
 */

#pragma once
#include "Object3D.h"
#include "Timer.h"

#include "CollisionManager3D.h"
#include "AABBCollider.h"
#include "RayCollider.h"

#include "NavMesh.h"
#include "Player.h"

#include <memory>

class Zombie {
public:
	// 状態
	enum class State {
		WAIT,
		WANDERING,
		CHASE,
		DEBUG,
	};

	// 役割
	enum class Role {
		Attacker,
		Defender,
	};

#pragma region メンバ変数
private:
	static CollisionManager3D* sColMgr_;
	static Model* sModel0_;
	static Model* sModel1_;
	static Player* sPlayer_;
	static NavMesh* sNavMesh_;
	static Vector3* sDebugNavGoal_;

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
	State state_ = State::WAIT;

	// HP
	uint8_t hp_ = 3;

	// 生存フラグ
	bool isAlive_ = true;
	bool isHead_ = false;
	bool isBody_ = false;

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
	Vector3 moveVec_ = { 0.0f, 0.0f, 0.0f };

	// ルート探索
	std::vector<Vector3> route_ = {};
	std::unique_ptr<Line3D> line_ = nullptr;
	float routeSearchInterval_ = 1.0f;// ルート探索のインターバル[s]
	uint64_t lastRouteSearchTime_ = 0;// 最後にルート探索した時間
	float speed_ = 0.0f;
	float defSpd_ = 0.05f;
	float rushSpd_ = 0.1f;
	float visualRecognitionDist_ = 40.0f;// 視認距離
	Vector3 wanderingPos_ = Vector3();// 徘徊位置
	float wanderingRadius_ = 5.5f;

	Vector3 knockBackVec_ = { 0.0f, 0.0f, 0.0f };
	float knockBackSpd_ = 0.0f;

	float hitReactionDuringTime_ = 0.2f;
	uint64_t hitTime_ = 0;

	bool isDebug_ = false;

	Timer waitT_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Zombie() {}

	// デストラクタ
	~Zombie();

	// 初期化処理
	void Initialize(const Vector3& inPos, const Vector3& inWanderingPos, State inState);

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

	void AddMoveVec(const Vector3& inVec) { moveVec_ += inVec; }

private:
	// 状態別処理
	static void (Zombie::* stateTable[]) ();
	void Wait();	// 待機
	void Wandering();// 徘徊
	void Chase();	// 追跡
	void DebugMove();// デバック時

	// ルート作成
	bool CreateRoute(const Vector3& goalPos);

	// 移動処理
	bool Move();
	bool RushMove();

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
	static void SetModel(Model* inModel0, Model* inModel1) { sModel0_ = inModel0, sModel1_ = inModel1; }
	static void SetPlayer(Player* inPlayer) { sPlayer_ = inPlayer; }
	static void SetNavMesh(NavMesh* inNavMesh) { sNavMesh_ = inNavMesh; }
	static void SetDebugNavGoal(Vector3* inDebugNavGoal) { sDebugNavGoal_ = inDebugNavGoal; }
	void SetRushMode();
#pragma endregion

#pragma region ゲッター関数
public:
	bool GetIsAlive() { return isAlive_; }
	bool GetIsHead() { return isHead_; }
	bool GetIsBody() { return isBody_; }
	const Vector3& GetPosition() { return object_->GetPosition(); }
#pragma endregion
};