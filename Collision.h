#pragma once
#include "float3.h"

struct SphereCol {
	float3 pos;
	float radius;
};

struct RayCol {
	float3 start;
	float3 end;
	Vector3 dir;
};