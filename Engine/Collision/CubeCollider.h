#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class CubeCollider : public BaseCollider, public Cube {
#pragma region メンバ変数
	// オブジェクト中心からのオフセット
	float3 offset_ = { 0.0f, 0.0f, 0.0f };

	// 各成分の半径
	float3 radius_ = { 0.0f, 0.0f, 0.0f };
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="offset"> オフセット </param>
	/// <param name="radius"> 半径 </param>
	CubeCollider(const float3& offset = { 0.0f, 0.0f, 0.0f }, const float3& radius = { 1.0f, 1.0f, 1.0f });

	// 更新処理
	void Update() override;
#pragma endregion
};