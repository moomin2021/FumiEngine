#pragma once
#include "Vector3.h"
#include <stdint.h>

class EnemyGenerator
{
#pragma region メンバ変数
private:
	float interval_ = 1.0f;// スポーン間隔
	uint64_t lastSpawnTime_ = 0;// 最後のスポーン時間
	bool isSpawn_ = false;// スポーンフラグ
	Vector3 offset_ = { 0.0f, 0.0f, 0.0f };
	Vector3 spawnPos_ = { 0.0f, 0.0f, 0.0f };// スポーン座標
	float minSpawn_ = 1.0f;// スポーン座標からの最小距離
	float maxSpawn_ = 3.0f;// スポーン座標からの最大距離
	uint16_t spawnNum_ = 0;// 残りのスポーンする数
#pragma endregion

#pragma region メンバ関数
public:
	EnemyGenerator() {}
	~EnemyGenerator();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();
#pragma endregion

#pragma region セッター関数
	void AddSpawnNum(uint16_t inSpawnNum) { spawnNum_ += inSpawnNum; }
	void SetInterval(float inInterval) { interval_ = inInterval; }
	void SetOffset(const Vector3& inPos) { offset_ = inPos; }
#pragma endregion

#pragma region ゲッター関数
	bool GetSpawnFrag() { return isSpawn_; }
	const Vector3& GetOffset() { return offset_; }
	const Vector3& GetSpawnPos() { return spawnPos_; }
#pragma endregion
};