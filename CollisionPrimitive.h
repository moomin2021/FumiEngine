#pragma once
#include <DirectXMath.h>

struct Sphere {
	// 中心座標
	DirectX::XMVECTOR center = { 0.0f, 0.0f, 0.0f, 1.0f };

	// 半径
	float radius = 1.0f;
};

struct Plane {
	// 法線ベクトル
	DirectX::XMVECTOR normal = { 0.0f, 1.0f, 0.0f, 0.0f };

	// 原点(0.0f, 0.0f, 0.0f)からの距離
	float distance = 0.0f;
};