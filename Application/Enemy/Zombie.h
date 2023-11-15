#pragma once
#include "Model.h"
#include "Object3D.h"
#include "CollisionManager.h"
#include "RayCollider.h"
#include "SphereCollider.h"
#include "Line3D.h"
#include "NavMesh.h"

#include "Player.h"

#include <memory>

class Zombie
{
private:
	enum class State {
		WAIT,
		PATROL,
		CHASE,
	};

#pragma region メンバ変数
private:
	static CollisionManager* sColMgr_;// 衝突判定管理クラス
	static Model* sModel_;// モデル
	static Player* sPlayer_;// プレイヤー
	static NavMesh* sNavMesh_;// ナビメッシュ

	// オブジェクト3D
	std::unique_ptr<Object3D> object_ = nullptr;

	// コライダー
	std::unique_ptr<RayCollider> cGroundJudgment_ = nullptr;// 接地判定を取るコライダー
	std::unique_ptr<SphereCollider> cSphere_ = nullptr;// ステージObjと衝突判定をする
	std::unique_ptr<RayCollider> cEnemy2Player_ = nullptr;// 敵からプレイヤーまでのレイ

	// ステート
	State state_ = State::WAIT;

	// 生存フラグ
	bool isAlive_ = true;

	// 接地している
	bool isGround_ = false;

	// 重力
	Vector3 gravity_ = { 0.0f, -1.0f, 0.0f };
	const float velocityLimit_ = 1.0f;
	float velocity_ = 0.0f;// 速度
	float accel_ = 0.05f;// 加速度

	// ゾンビの向き
	float turnSpd_ = 4.0f;
	float angle_ = 0.0f;
	Vector2 forwardVec_ = { 0.0f, 1.0f };

	// ルート探索
	std::unique_ptr<Line3D> line_ = nullptr;
	std::vector<Vector3> route_ = {};
	float routeSearchInterval_ = 1.0f;// ルート探索のインターバル[s]
	uint64_t lastRouteSearchTime_ = 0;// 最後にルート探索した時間
	float moveSpd_ = 0.1f;
	float visualRecognitionDist_ = 40.0f;// 視認距離

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

	// デバック処理
	void Debug();

private:
	// 状態別処理
	static void (Zombie::* stateTable[]) ();
	void Wait();		// 待機状態
	void Patrol();		// 見回り状態
	void Chase();		// 追跡状態

	void GroundingJudgment();// 接地判定
	void Gravity();// 重力処理
	void Rotate();// 回転処理
	void CreateNavRoute();// ルート作成
#pragma endregion

#pragma region セッター関数
public:
	static void SetCollisionManager(CollisionManager* inColMgr) { sColMgr_ = inColMgr; }
	static void SetModel(Model* inModel) { sModel_ = inModel; }
	static void SetPlayer(Player* inPlayer) { sPlayer_ = inPlayer; }
	static void SetNavMesh(NavMesh* inNavMesh) { sNavMesh_ = inNavMesh; }
#pragma endregion

#pragma region ゲッター関数
public:
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};