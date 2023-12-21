#pragma once
#include <stdint.h>

class DeltaTime
{
#pragma region メンバ変数
private:
	uint64_t oldTimeMS_ = 0;
	float deltaTime_ = 0.0f;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();
#pragma endregion

#pragma region ゲッター関数
	float GetDeltaTime() { return deltaTime_; }
#pragma endregion
};