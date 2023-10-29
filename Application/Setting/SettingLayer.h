#pragma once
#include "BoxButton.h"

#include <memory>
#include <vector>

class SettingLayer {
#pragma region メンバ変数
private:
	// ボタン
	std::vector<std::unique_ptr<BoxButton>> buttons_ = {};

	// 設定一覧の背景画像
	std::unique_ptr<Sprite> sSettingBackground_ = nullptr;
	uint16_t gSettingBackground_ = 0;

	// 描画フラグ
	bool isDraw_ = true;
#pragma endregion

#pragma region メンバ関数
public:
	SettingLayer() {}
	~SettingLayer();

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