#pragma once
#include <stdint.h>

class ResultData
{
#pragma region メンバ変数
public:
	uint16_t elapsedTime_ = 0;
	uint16_t killEnemy_ = 0;
	uint16_t breakCore_ = 0;
	bool isWin_ = false;
#pragma endregion

#pragma region メンバ関数
	ResultData() {}
	~ResultData() {}

	// インスタンス取得
	static ResultData* GetInstance();
#pragma endregion

#pragma region 特殊関数
	ResultData(const ResultData&) = delete;				// コピーコンストラクタ禁止
	ResultData& operator = (const ResultData&) = delete;	// コピー代入演算子禁止
#pragma endregion
};