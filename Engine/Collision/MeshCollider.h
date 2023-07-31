#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class MeshCollider : public BaseCollider {
#pragma region メンバ変数
private:
	// 三角形
	std::vector<Triangle> triangles_;

	// ワールド行列の逆行列
	Matrix4 invMatWorld_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MeshCollider(Object3D* object);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 球との当たり判定
	/// </summary>
	/// <param name="sphere"> 球 </param>
	/// <param name="inter"> 交点(出力用) </param>
	/// <returns> 交差しているか否か </returns>
	bool CheckCollisionSphere(const Sphere& sphere, Vector3* inter = nullptr, Vector3* reject = nullptr);

	/// <summary>
	/// レイとの当たり判定
	/// </summary>
	/// <param name="ray"> レイ </param>
	/// <param name="distance"> 距離(出力用) </param>
	/// <param name="inter"> 交点(出力用) </param>
	/// <returns> 交差しているか否か </returns>
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, Vector3* inter = nullptr);

private:
	/// <summary>
	/// 三角形の配列を構築する
	/// </summary>
	/// <param name="model"> モデル </param>
	void ConstructTriangles(Model* model);
#pragma endregion
};