#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "Boss.h"

#include <memory>

class BossGenerator
{
#pragma region メンバ変数
private:
	// ジェネレーター
	std::unique_ptr<Model> mGen_;// モデル
	std::unique_ptr<Object3D> oGen_;// オブジェクト
	std::unique_ptr<SphereCollider> cGen_;// コライダー

	// ボス
	std::unique_ptr<Boss> boss_;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	BossGenerator();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
#pragma endregion
};