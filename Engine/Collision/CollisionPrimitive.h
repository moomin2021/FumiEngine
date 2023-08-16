#pragma once
#include "Vector3.h"
#include "float3.h"

// 球
struct Sphere {
	// 中心座標
	Vector3 center = { 0.0f, 0.0f, 0.0f };

	// 半径
	float radius = 1.0f;
};

// 平面
struct Plane {
	// 法線ベクトル
	Vector3 normal = { 0.0f, 1.0f, 0.0f };

	// 原点(0.0f, 0.0f, 0.0f)からの距離
	float distance = 0.0f;
};

// 三角形
class Triangle {
public:
	// 頂点座標3つ
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;

	// 法線ベクトル
	Vector3 normal;

	void ComputeNormal();
};

struct Ray {
	// 始点座標
	Vector3 start = { 0.0f, 0.0f, 0.0f };

	// 方向
	Vector3 dir = { 1.0f, 0.0f, 0.0f };
};

struct Cube {
	// 中心座標
	Vector3 center = { 0.0f, 0.0f, 0.0f };

	// 各成分の半径
	float3 radius = { 1.0f, 1.0f, 1.0f };
};