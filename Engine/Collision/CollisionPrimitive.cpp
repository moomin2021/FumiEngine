﻿#include "CollisionPrimitive.h"

void Triangle::ComputeNormal() {
	Vector3 p0_p1 = p1 - p0;
	Vector3 p0_p2 = p2 - p0;

	// 外積により、2辺に垂直なベクトルを算出する
	normal = Vector3Cross(p0_p1, p0_p2);
	normal = Vector3Normalize(normal);
}
