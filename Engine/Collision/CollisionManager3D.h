/**
 * @file Player.h
 * @brief 3Dの衝突判定の管理蔵ｓｙ
 * @author moomin
 * @date 2024/01/16
 */

#pragma once
#include "BaseCollider.h"
#include "AABBCollider.h"
#include "RayCollider.h"

#include <forward_list>

struct ColData {
	Vector3 offset = Vector3();
	Vector3 radius = Vector3();
	Vector3 rayDir = Vector3();
};

class CollisionManager3D
{
#pragma region メンバ変数
private:
	// 押し戻し用のレイコライダー
	std::forward_list<RayCollider*> pushBackRayColliders_ = {};

	// ブロックのコライダー
	std::forward_list<AABBCollider*> blockColliders_ = {};

	// AABBのコライダー
	std::forward_list<AABBCollider*> aabbColliders_ = {};

	// Rayのコライダー
	std::forward_list<RayCollider*> rayColliders_ = {};
#pragma endregion 

#pragma region メンバ関数
public:
	// すべてのコライダーの衝突判定
	void CheckAllCollision();

	// 押し戻し用のレイコライダー追加
	void AddPushBackRayCollider(RayCollider* collider) { pushBackRayColliders_.emplace_front(collider); }

	// コライダー追加
	void AddCollider(BaseCollider* collider);

	// ブロックコライダー追加
	void AddBlockCollider(AABBCollider* collider) { blockColliders_.emplace_front(collider); }

	// 押し戻し用のレイコライダー削除
	void RemovePushBackRayCollider(RayCollider* collider) { pushBackRayColliders_.remove(collider); }

	// コライダー削除
	void RemoveCollider(BaseCollider* collider);

	// ブロックコライダー削除
	void RemoveBlockCollider(AABBCollider* collider) { blockColliders_.remove(collider); }

	// 後処理
	void Release();

private:
	// コンストラクタ
	CollisionManager3D() {}

	// デストラクタ
	~CollisionManager3D();

	// コライダーの更新
	void ColliderUpdate();

	// 押し戻し用RayとAABBの衝突判定処理
	void PushBackRayAndBlockCollision();

	// AABB同士の衝突判定
	void AABBCollision();

	// ブロックとAABBの衝突判定
	void BlockAndAABBCollision();

	// Rayとその他コライダーの衝突判定
	void RayAndOthersCollision();
#pragma endregion

#pragma region ゲッター関数
public:
	// インスタンスを取得
	static CollisionManager3D* GetInstance();
#pragma endregion

#pragma region 特殊関数
	CollisionManager3D(const CollisionManager3D&) = delete;				// コピーコンストラクタ禁止
	CollisionManager3D& operator = (const CollisionManager3D&) = delete;	// コピー代入演算子禁止
#pragma endregion
};