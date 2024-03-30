#pragma once
#include "BaseEnemy.h"
#include "NavMesh.h"

class Player;

struct ColData {
	Vector3 offset = Vector3();
	Vector3 radius = Vector3();
	Vector3 rayDir = Vector3();
};

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

	bool isGround_ = false;
	Vector3 gravityVec_ = { 0.0f, -1.0f, 0.0f };
	float gravity_ = 0.0f;
	const float gravityLimit_ = 1.0f;
	const float accel_ = 0.05f;

	std::vector<Vector3> route_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	Zombie(CollisionManager3D* inColMgr, NavMesh* inNavMesh, Player* inPlayer, Model* inModel0, Model* inModel1);
	~Zombie() {}

	void Initialize(const Vector3& inPos) override;
	void Update() override;
	void Draw() override;
	void Collision() override;
	void MatUpdate() override;
	void Finalize() override;

private:
	// 状態別処理
	static void (Zombie::*stateTable[]) ();
	void Wait();
	void Chase();

	void Rotate();
	void Gravity();
	void GroundingJudgment();
#pragma endregion
};