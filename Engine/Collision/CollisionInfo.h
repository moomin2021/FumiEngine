#pragma once
#include "Vector3.h"

class Object3D;
class BaseCollider;

struct CollisionInfo {
#pragma region メンバ変数
public:
	// 衝突相手のオブジェクト
	Object3D* object = nullptr;

	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;

	// 衝突点
	Vector3 inter;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="object"> 衝突相手のオブジェクト </param>
	/// <param name="collider"> 衝突相手のコライダー </param>
	/// <param name="inter"> 衝突点 </param>
	CollisionInfo(Object3D* object, BaseCollider* collider, const Vector3& inter) {
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}
#pragma endregion
};