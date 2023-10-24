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

	// タイトル
	std::unique_ptr<Sprite> sTitle_ = nullptr;
	uint16_t gTitle_ = 0;

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
	void OnCollision(ButtonAttribute& buttonAttr);

	// 行列更新処理
	void MatUpdate();

	// 衝突判定の[ON][OFF]を切り替える
	void SetIsCollision(bool frag);
#pragma endregion
};