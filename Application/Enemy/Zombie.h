/**
 * @file EnemyManager.h
 * @brief ゾンビクラス
 * @author moomin
 * @date 2024/04/17
 */

 // このクラスのインスタンスは初期化する前に各データのポインタを取得しておく必要がある
 // 以下取得初期化前に取得しておくべきデータ一覧
 // CollisionManager3D
 // NavMesh
 // Player
 // Model

#pragma once
#include "BaseEnemy.h"
#include "NavMesh.h"
#include "CollisionManager3D.h"
#include "Line3D.h"

class Player;

class Zombie : public BaseEnemy
{
private:
	enum class State {
		WAIT,
		CHASE,
	};

#pragma region メンバ変数
private:
	NavMesh* pNavMesh_ = nullptr;
	Player* pPlayer_ = nullptr;
	Model* model0_ = nullptr;
	Model* model1_ = nullptr;

	ColData headColData_;
	ColData bodyColData_;
	ColData groundColData_;

	State state_ = State::WAIT;

	const float rotaSpd_ = 8.0f;
	Vector3 moveVec_ = Vector3();
	float angle_ = 0.0f;
	float speed_ = 0.05f;

	bool isGround_ = false;
	Vector3 gravityVec_ = { 0.0f, -1.0f, 0.0f };
	float gravity_ = 0.0f;
	const float gravityLimit_ = 1.0f;
	const float accel_ = 0.05f;

	float routeSearchInterval_ = 5.0f;
	float randomRange_ = 20.0f;
	std::vector<Vector3> route_ = {};
	std::unique_ptr<Line3D> line_ = nullptr;

	bool isDebug_ = false;
#pragma endregion

#pragma region メンバ関数
public:
	Zombie() : BaseEnemy() {}
	~Zombie() {}

	void Initialize(const Vector3& inPos) override;
	void Update() override;
	void Draw() override;
	void Collision() override;
	void MatUpdate() override;
	void Finalize() override;
	void Debug(bool isDebug) override;

private:
	// 状態別処理
	static void (Zombie::*stateTable[]) ();
	void Wait();
	void Chase();
	
	void Move();
	void Rotate();
	void Gravity();
	void GroundingJudgment();
	bool CreateRoute(const Vector3& goal);
	void RandomRoute();
#pragma endregion

#pragma region セッター関数
public:
	void SetCollisionManager3D(CollisionManager3D* inColMgr) { colMgr_ = inColMgr; }
	void SetNavMesh(NavMesh* inNavMesh) { pNavMesh_ = inNavMesh; }
	void SetPlayer(Player* inPlayer) { pPlayer_ = inPlayer; }
	void SetModel(Model* inModel0, Model* inModel1) { model0_ = inModel0, model1_ = inModel1; }
#pragma endregion
};