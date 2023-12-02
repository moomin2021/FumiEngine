#include "EnemyGenerator.h"
#include "Util.h"

EnemyGenerator::~EnemyGenerator()
{

}

void EnemyGenerator::Initialize()
{

}

void EnemyGenerator::Update()
{
	isSpawn_ = false;

	// 前回スポーンしてからの経過時間
	float elapsed = (Util::GetTimrMSec() - lastSpawnTime_) / 1000.0f;

	if (elapsed >= interval_ && spawnNum_ > 0)
	{
		isSpawn_ = true;
		spawnNum_ -= 1;
		lastSpawnTime_ = Util::GetTimrMSec();
		spawnPos_.x = (Util::GetRandomFloat(minSpawn_, maxSpawn_) * (Util::GetRandomInt(0, 1) * 2 - 1)  ) + offset_.x;
		spawnPos_.y = offset_.y;
		spawnPos_.z = (Util::GetRandomFloat(minSpawn_, maxSpawn_) * (Util::GetRandomInt(0, 1) * 2 - 1)) + offset_.z;
	}
}