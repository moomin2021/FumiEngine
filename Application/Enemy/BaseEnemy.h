#pragma once
#include "Object3D.h"
#include "SphereCollider.h"

#include <memory>

class BaseEnemy
{
#pragma region メンバ変数
protected:
	// オブジェクト
	std::unique_ptr<Object3D> object_;

	// コライダー
	std::unique_ptr<SphereCollider> collider_;
#pragma endregion

#pragma region メンバ関数
public:
	// 更新処理
	virtual void Update() = 0;

	// 描画処理
	virtual void Draw() = 0;
#pragma endregion
};