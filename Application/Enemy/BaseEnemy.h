#pragma once
#include "Object3D.h"

#include "CollisionManager3D.h"
#include "AABBCollider.h"
#include "RayCollider.h"

#include <memory>

class BaseEnemy
{
#pragma region メンバ変数
protected:
	CollisionManager3D* colMgr_;

	uint8_t nowHP_ = 0;
	bool isAlive_ = true;
	std::unique_ptr<Object3D> object_ = nullptr;

	std::unique_ptr<AABBCollider> headC_ = nullptr;
	std::unique_ptr<AABBCollider> bodyC_ = nullptr;
	std::unique_ptr<RayCollider> groundC_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	BaseEnemy() {}
	virtual ~BaseEnemy() {}

	virtual void Initialize(const Vector3& inPos) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Collision() = 0;
	virtual void MatUpdate() = 0;
	virtual void Finalize() = 0;
	virtual void Debug(bool isDebug) = 0;

protected:
	void HeadHit();
	void BodyHit();
	void JudDeath();
#pragma endregion

#pragma region ゲッター関数
public:
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};