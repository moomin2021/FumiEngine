#pragma once
#include "Key.h"
#include "Mouse.h"

class Player
{
#pragma region メンバ変数
private:
	// 入力クラスインスタンス
	Key* key_;		// キーボード
	Mouse* mouse_;	// マウス
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// プレイヤー
	/// </summary>
	Player();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
#pragma endregion
};