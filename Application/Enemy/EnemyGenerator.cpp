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
	float elapsed = (Util::GetTimrMSec() - spawnTime_) / 1000.0f;

	if (elapsed >= spawnInterval_)
	{
		isSpawn_ = true;
		spawnTime_ = Util::GetTimrMSec();
	}
}