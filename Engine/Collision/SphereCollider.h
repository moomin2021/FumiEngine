#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "float3.h"

class SphereCollider : public BaseCollider, public Sphere
{
#pragma region メンバ変数
private:
	// オブジェクト中心からのオフセット
	float3 offset_;

	// 半径
	float radius_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="offset"> オフセット </param>
	/// <param name="radius"> 半径 </param>
	SphereCollider(float3 offset = { 0.0f, 0.0f, 0.0f }, float radius = 1.0f);

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
	/// 半径を設定
	/// </summary>
	/// <param name="radius"> 半径 </param>
	void SetRadius(float radius) { radius_ = radius; }
#pragma endregion
};