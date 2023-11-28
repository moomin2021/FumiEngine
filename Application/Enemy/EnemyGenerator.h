#pragma once
#include <stdint.h>

class EnemyGenerator
{
#pragma region メンバ変数
private:
	// 敵のスポーン間隔
	float spawnInterval_ = 10.0f;
	uint64_t spawnTime_ = 0;
	bool isSpawn_ = false;
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

#pragma region ゲッター関数
	bool GetSpawnFrag() { return isSpawn_; }
#pragma endregion
};