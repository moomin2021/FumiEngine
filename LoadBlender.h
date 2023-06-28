#pragma once
#include <string>
#include <map>
#include <vector>

class LoadBlender
{
#pragma region 構造体

#pragma endregion

#pragma region メンバ変数

#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LoadBlender();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// Jsonファイル読み込み
	/// </summary>
	void LoadJson(std::string fileName);
#pragma endregion
};