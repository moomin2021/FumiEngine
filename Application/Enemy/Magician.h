#pragma once
#include "Model.h"
#include "Object3D.h"
#include "CollisionManager.h"
#include "RayCollider.h"
#include "SphereCollider.h"
#include "Line3D.h"
#include "NavMesh.h"
#include "AABBCollider.h"

#include "Player.h"
#include "MagicianBullet.h"

#include <memory>
#include <deque>

class Magician
{
private:
	enum class State {
		WAIT,
		PATROL,
		CHASE,
		MAGIC,
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
	std::unique_ptr<RayCollider> cEnemy2Player_ = nullptr;// 敵からプレイヤーまでのレイ
	std::unique_ptr<AABBCollider> cHit_ = nullptr;// プレイヤーの玉などと衝突判定をとる

	// ステート
	State state_ = State::CHASE;

	// HP
	int8_t hp_ = 3;

	// ゾンビの高さ
	float height_ = 2.0f;

	// 生存フラグ
	bool isAlive_ = true;

	// 接地している
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
	//std::unique_ptr<Line3D> line_ = nullptr;
	float routeSearchInterval_ = 1.0f;// ルート探索のインターバル[s]
	uint64_t lastRouteSearchTime_ = 0;// 最後にルート探索した時間
	float moveSpd_ = 0.05f;
	float visualRecognitionDist_ = 40.0f;// 視認距離
	float range_ = 10.0f;// 射程

	float coolDown_ = 5.0f;
	uint64_t startCoolDown_ = 0;

	Vector3 knockBackVec_ = { 0.0f, 0.0f, 0.0f };
	float knockBackSpd_ = 0.0f;

	float hitReactionDuringTime_ = 0.2f;
	uint64_t hitTime_ = 0;

	// プレイヤーを視認しているか
	bool isLookingPlayer_ = false;

	// 魔法
	std::deque<std::unique_ptr<MagicianBullet>> bullet_ = {};

#pragma endregion

#pragma region メンバ関数
public:
	Magician() {}
	~Magician();

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
	static void (Magician::* stateTable[]) ();
	void Wait();		// 待機状態
	void Patrol();		// 見回り状態
	void Chase();		// 追跡状態
	void Magic();		// 魔法状態

	void GroundingJudgment();// 接地判定
	void Gravity();// 重力処理
	void Rotate();// 回転処理
	void CreateNavRoute();// ルート作成
	void Jump();
	void LookingPlayer();// プレイヤーの間に障害物がないか
	void BulletUpdate();
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
	const Vector3& GetPosition() { return object_->GetPosition(); }
#pragma endregion
};