/**
 * @file Player.h
 * @brief プレイヤーのクラス
 * @author moomin
 * @date 2023/12/29
 */

#pragma once
#include "Key.h"
#include "Mouse.h"

#include "CollisionManager3D.h"
#include "AABBCollider.h"
#include "RayCollider.h"

#include "Object3D.h"
#include "Model.h"
#include "Camera.h"

#include "Weapon.h"

#include <memory>

class Player
{
#pragma region メンバ変数
private:
	// シングルトンインスタンス
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;
	CollisionManager3D* colMgr_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Object3D> object_ = nullptr;
	std::unique_ptr<Weapon> weapon_ = nullptr;

	std::unique_ptr<AABBCollider> playerC_ = nullptr;
	std::unique_ptr<RayCollider> groundC_ = nullptr;
	std::unique_ptr<RayCollider> shotC_ = nullptr;

	ColData playerColData_ = {};
	ColData groundColData_ = {};
	ColData shotColData_ = {};

	std::unique_ptr<Camera> camera_ = nullptr;
	Vector3 cameraOffset_ = Vector3();
	Vector3 eyeAngle_ = Vector3(0.0f, 90.0f, 0.0f);
	const float sencivity_ = 0.1f;
	float nowFovAngle_ = 70.0f;
	const float fovAngleIncrDecValue_ = 2.0f;
	const float maxFovAngle_ = 90.0f;
	const float minFovAngle_ = 40.0f;
	const float defFovAngle_ = 70.0f;
	const float maxRecoilEyeAngle_ = 10.0f;// 最大リコイル角度
	const float maxDiffusivity_ = 10.0f;// 最大拡散
	float nowDiffusivity_ = 10.0f;// 現在の拡散
	float nowRecoilEyeAngle_ = 0.0f;// リコイル角度
	float decRecoilEyeAngle_ = 1.0f;// リコイル角度の減少値

	bool isAlive_ = true;

	float moveSpd_ = 0.0f;
	const float moveAcc_ = 0.05f;
	const float maxMoveSpd_ = 0.15f;
	const float maxDashSpd_ = 0.25f;
	bool isDash_ = false;

	const uint16_t maxHP_ = 10;
	uint16_t nowHP_ = maxHP_;

	Vector3 forwardVec_ = Vector3();
	Vector3 rightVec_ = Vector3();

#pragma endregion

#pragma region メンバ関数
public:
	void Initialize(const Vector3& inPos);
	void Update();
	void Draw();
	void Collision();
	void MatUpdate();
	void Finalize();

private:
	void EyeMove();
	void Move();
	void Dash();
	void Shot();
	void FovAngleUpdate();
	void JudgmentDeath();
	void CameraUpdate();
#pragma endregion

#pragma region ゲッター関数
public:
	const Vector3& GetPosition() { return object_->GetPosition(); }
	uint16_t GetMaxHP() { return maxHP_; }
	uint16_t GetNowHP() { return nowHP_; }
	bool GetIsAlive() { return isAlive_; }
	Camera* GetCamera() { return camera_.get(); }
	Weapon* GetWeapon() { return weapon_.get(); }
#pragma endregion
};