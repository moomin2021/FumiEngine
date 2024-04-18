#pragma once
class GameSettingData
{
#pragma region メンバ変数
public:
	float sensitivity_ = 1.0f;
	float bgm_ = 1.0f;
	float se_ = 1.0f;
#pragma endregion

#pragma region メンバ関数
public:
	static GameSettingData* GetInstance();

private:
	GameSettingData() {}
	~GameSettingData() {}
#pragma endregion

#pragma region 特殊関数
	GameSettingData(const GameSettingData&) = delete;				// コピーコンストラクタ禁止
	GameSettingData& operator = (const GameSettingData&) = delete;	// コピー代入演算子禁止
#pragma endregion
};