#pragma once
#include <stdint.h>

class DeltaTime
{
#pragma region メンバ変数
private:
	uint64_t oldTimeMS_ = 0;
	float deltaTime_ = 0.0f;
	float timeSpd_ = 1.0f;
	float gameTime_ = 0.0f;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	static DeltaTime* GetInstance();

private:
	DeltaTime() {}
	~DeltaTime() {}
#pragma endregion

#pragma region セッター関数
public:
	void SetTimeSpeed(float inTimeSpd) { timeSpd_ = inTimeSpd; }
#pragma endregion

#pragma region ゲッター関数
public:
	float GetDeltaTime() { return deltaTime_; }
	float GetGameTime() { return gameTime_; }
	float GetTimeSpeed() { return timeSpd_; }
#pragma endregion

#pragma region 特殊関数
	DeltaTime(const DeltaTime&) = delete;				// コピーコンストラクタ禁止
	DeltaTime& operator = (const DeltaTime&) = delete;	// コピー代入演算子禁止
#pragma endregion
};