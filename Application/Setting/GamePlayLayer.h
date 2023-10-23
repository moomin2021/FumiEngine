#pragma once
#include "BaseLayer.h"
#include "Sprite.h"
#include "BaseCollider2D.h"
#include "CircleCollider.h"
#include "PointCollider.h"
#include "CollisionManager2D.h"
#include "Mouse.h"

#include <memory>

class GamePlayLayer : public BaseLayer
{
#pragma region メンバ変数
	// インスタンス
	Mouse* mouse_ = nullptr;
	CollisionManager2D* colMgr2D_ = nullptr;

	// スライダー
	std::unique_ptr<Sprite> sSlider_ = nullptr;
	std::unique_ptr<Sprite> sSliderPoint_ = nullptr;
	uint16_t gSlider_ = 0;
	uint16_t gSliderPoint_ = 0;

	// コライダー
	std::unique_ptr<PointCollider> cMouse_ = nullptr;
	std::unique_ptr<CircleCollider> cSliderPoint_ = nullptr;
#pragma endregion

#pragma region メンバ関数
	GamePlayLayer() {}
	~GamePlayLayer();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	// 衝突判定の[ON][OFF]を切り替える
	//void SetIsCollision(bool frag);
#pragma endregion
};