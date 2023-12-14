#pragma once
#include "Sprite.h"

#include <memory>

class PlayerUI
{
#pragma region メンバ変数
private:
	// スプライト
	std::unique_ptr<Sprite> hpBarS_ = nullptr;
	std::unique_ptr<Sprite> hpFrameS_ = nullptr;
	std::unique_ptr<Sprite> hpFrameShadowS_ = nullptr;

	// 画像ハンドル
	uint16_t hpBarH_ = 0;
	uint16_t hpFrameH_ = 0;
	uint16_t hpFrameShadowH_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	PlayerUI() {}
	~PlayerUI();

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
#pragma endregion
};