#pragma once
#include "CollisionPrimitive.h"
#include "RaycastHit.h"

#include <forward_list>

class BaseCollider;

class CollisionManager {
#pragma region メンバ変数
private:
	// コライダーリスト
	std::forward_list<BaseCollider*> colliders_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns> インスタンス </returns>
	static CollisionManager* GetInstance();

	/// <summary>
	/// 全ての衝突チェック
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// レイキャスト
	/// </summary>
	/// <param name="ray"> レイ </param>
	/// <param name="hitinfo"> 衝突情報 </param>
	/// <param name="maxDistance"> 最大距離 </param>
	/// <returns> レイが任意のコライダーと交わる場合はtrue </returns>
	bool Raycast(const Ray& ray, RaycastHit* hitinfo = nullptr, float maxDistance = FLT_MAX);

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider"> コライダー </param>
	inline void AddCollider(BaseCollider* collider) { colliders_.push_front(collider); }

	/// <summary>
	/// コライダーの削除
	/// </summary>
	/// <param name="collider"></param>
	inline void RemoveCollider(BaseCollider* collider) { colliders_.remove(collider); }
#pragma endregion

#pragma region 特殊関数
private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;
#pragma endregion
};