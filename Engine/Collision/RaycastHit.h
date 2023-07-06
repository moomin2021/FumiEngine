#pragma once
#include "BaseCollider.h"

class Object3D;

struct RaycastHit {
	// 衝突相手のオブジェクト
	Object3D* object = nullptr;

	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;

	// 衝突点
	Vector3 inter;

	// 衝突点までの距離
	float distance = 0.0f;
};