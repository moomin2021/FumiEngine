#pragma once
#include "BaseLayer.h"
#include "BoxButton.h"
#include "HitFrame.h"

#include <vector>
#include <memory>

class TitleLayer : public BaseLayer
{
#pragma region メンバ変数
private:
	// ボタン
	std::vector<std::unique_ptr<BoxButton>> buttons_ = {};

	// フレーム関連
	uint16_t frameHandle_ = 0;
	const Vector2 frameSize_ = { 324.0f, 54.0f };
	std::unique_ptr<HitFrame> hitFrame_ = nullptr;

#pragma endregion

#pragma region メンバ関数
public:
	TitleLayer() {}
	~TitleLayer();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision(SelectNum& selectNum);

	// 行列更新処理
	void MatUpdate();

	// 衝突判定の[ON][OFF]を切り替える
	void SetIsCollision(bool frag);
#pragma endregion
};