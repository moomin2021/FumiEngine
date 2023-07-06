#pragma once
#include "Object3D.h"
#include "Key.h"

class Player : public Object3D
{
#pragma region メンバ変数
	// キーボード入力
	Key* key_;
#pragma endregion

#pragma region メンバ関数
public:
	Player(Model* model) : Object3D(model) {}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info"></param>
	void OnCollision(const CollisionInfo& info) override;
#pragma endregion
};

