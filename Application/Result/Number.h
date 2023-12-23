#pragma once
#include "Vector2.h"

#include "Sprite.h"

#include <vector>
#include <string>
#include <memory>

class Number
{
#pragma region メンバ変数
private:
	// スプライト
	std::vector<std::unique_ptr<Sprite>> numS_ = {};

	// 画像
	std::vector<uint16_t> handles_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化処理
	void Initialize(uint16_t digit, const Vector2& inPos, const Vector2& inSize, const std::string numFileName);

	// 更新処理
	void MatUpdate();

	// 描画処理
	void Draw(uint16_t inValue);
#pragma endregion
};