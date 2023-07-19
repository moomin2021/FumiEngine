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

	// 生存フラグ
	bool isAlive_ = true;
#pragma endregion

#pragma region メンバ関数
public:
	// 更新処理
	virtual void Update() = 0;

	// 描画処理
	virtual void Draw() = 0;
#pragma endregion

#pragma region ゲッター関数
	/// <summary>
	/// 生存フラグを取得
	/// </summary>
	/// <returns> フラグ </returns>
	inline bool GetIsAlive() { return isAlive_; }
#pragma endregion
};