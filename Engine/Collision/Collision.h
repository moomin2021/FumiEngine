#pragma once
#include "CollisionPrimitive.h"

class Collision {
#pragma region メンバ関数
public:
	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphere0"> 球0 </param>
	/// <param name="sphere1"> 球1 </param>
	/// <param name="inter">  </param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphere0, const Sphere& sphere1, Vector3* inter = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere"> 球 </param>
	/// <param name="plane"> 平面 </param>
	/// <param name="inter"> 交点(平面上の最近接点) </param>
	/// <returns> 交差しているか否か </returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point"> 点 </param>
	/// <param name="traiangle"> 三角形 </param>
	/// <param name="closest"> 最近接点(出力用) </param>
	/// <returns>  </returns>
	static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

	/// <summary>
	/// 球と法線付き三角形の当たり判定チェック
	/// </summary>
	/// <param name="sphere"> 球 </param>
	/// <param name="triangle"> 三角形 </param>
	/// <param name="inter"> 交点(三角形上の最近接点) </param>
	/// <returns> 交差しているか否か </returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="ray"> レイ </param>
	/// <param name="plane"> 平面 </param>
	/// <param name="distance"> 距離(出力用) </param>
	/// <param name="inter"> 交点(出力用) </param>
	/// <returns> 交差しているか否か </returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="ray"> レイ </param>
	/// <param name="triangle"> 三角形 </param>
	/// <param name="distance"> 距離(出力用) </param>
	/// <param name="inter"> 交点(出力用) </param>
	/// <returns> 交差しているか否か </returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="ray"> レイ </param>
	/// <param name="sphere"> 球 </param>
	/// <param name="distance"> 距離(出力用) </param>
	/// <param name="inter"> 交点(出力用) </param>
	/// <returns> 交差しているか否か </returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);
#pragma endregion
};