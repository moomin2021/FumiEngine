#pragma once
#include "BaseEnemy.h"
#include "Object3D.h"
#include "SphereCollider.h"

#include <memory>

class Enemy0 : public BaseEnemy
{
#pragma region メンバ変数
private:
	// HP
	uint16_t hp_;

	uint16_t damageCounter_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model"> モデル </param>
	Enemy0(Model* model);

	// デストラクタ
	~Enemy0();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos"> 初期位置 </param>
	void Initialize(float3 pos, float3 scale);

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

private:
	// 衝突時の処理
	void OnCollision();
#pragma endregion
};