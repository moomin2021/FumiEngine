#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class SphereCollider : public BaseCollider, public Sphere {
#pragma region メンバ変数
private:
	// オブジェクト中心からのオフセット
	Vector3 offset_;

	// 半径
	float radius_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="offset"> オブジェクト中心からのオフセット </param>
	/// <param name="radius"> 半径 </param>
	SphereCollider(Vector3 offset = { 0.0f, 0.0f, 0.0f }, float radius = 1.0f) :
		offset_(offset), radius_(radius)
	{
		// 球形状をセット
		shapeType_ = SHAPE_SPHERE;
	}

	// 更新処理
	void Update() override;
#pragma endregion

#pragma region セッター関数
	/// <summary>
	/// 半径を設定
	/// </summary>
	/// <param name="radius"> 半径 </param>
	inline void SetRadius(float radius) { radius_ = radius; }
#pragma endregion
};