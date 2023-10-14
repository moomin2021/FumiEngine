#pragma once
#include "BaseLayer.h"
#include "BoxButton.h"

#include <vector>
#include <memory>

class TitleLayer : public BaseLayer
{
#pragma region メンバ変数
private:
	// ボタン
	std::vector<std::unique_ptr<BoxButton>> buttons_ = {};
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
	void OnCollision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion
};