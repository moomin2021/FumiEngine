#pragma once
#include "BoxButton.h"
#include "HitFrame.h"

#include <vector>
#include <memory>

class TitleLayer
{
#pragma region メンバ変数
private:
	// ボタン
	std::vector<std::unique_ptr<BoxButton>> buttons_ = {};

	// タイトル
	std::unique_ptr<Sprite> sTitle_ = nullptr;
	uint16_t gTitle_ = 0;

	// 描画フラグ
	bool isDraw_ = true;
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

	// 表示の[ON][OFF]を切り替える
	void SetIsDisplay(bool frag);
#pragma endregion
};