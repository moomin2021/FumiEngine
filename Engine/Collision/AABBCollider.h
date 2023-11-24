#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class AABBCollider : public BaseCollider, public AABB
{
#pragma region メンバ変数
private:
	// オブジェクト中心からのオフセット
	Vector3 offset_ = { 0.0f, 0.0f, 0.0f };
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	AABBCollider();
	AABBCollider(const Vector3& inOffset, const Vector3& inRadius = { 0.0f, 0.0f, 0.0f });

	// 更新処理
	void Update() override;
#pragma endregion

#pragma region セッター関数
public:
	// オフセットを設定
	inline void SetOffset(const Vector3& offset) { offset_ = offset; }

	// 半径を設定
	inline void SetRadius(const Vector3& inRadius) { AABB::radius = inRadius; }
#pragma endregion
};
