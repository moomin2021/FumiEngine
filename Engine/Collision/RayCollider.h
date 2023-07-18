#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "float3.h"

class RayCollider : public BaseCollider, public Ray
{
#pragma region メンバ変数
	// オブジェクト中心からのオフセット
	float3 offset_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="offset"> オフセット </param>
	/// <param name="radius"> 半径 </param>
	RayCollider(float3 offset = { 0.0f, 0.0f, 0.0f }, Vector3 dir = { 0.0f, 0.0f, 1.0f });

	// 更新処理
	void Update() override;
#pragma endregion

#pragma region セッター関数
public:
	/// <summary>
	/// オフセットを設定
	/// </summary>
	/// <param name="offset"> オフセット </param>
	void SetOffset(const float3& offset) { offset_ = offset; }

	/// <summary>
	/// 方向を設定
	/// </summary>
	/// <param name="dir"> 方向 </param>
	void SetDir(const Vector3& dir) { Ray::dir = dir; }
#pragma endregion
};