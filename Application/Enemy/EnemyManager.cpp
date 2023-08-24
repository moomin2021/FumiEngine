#include "EnemyManager.h"
#include "CollisionAttribute.h"

EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager()
{
	colMgr_->RemoveCollider(colBossGenerator_.get());
}

void EnemyManager::Initialize()
{
#pragma region インスタンス
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region モデル
	mBossGenerator_ = std::make_unique<Model>("bossGenerator");
#pragma endregion

#pragma region オブジェクト
	oBossGenerator_ = std::make_unique<Object3D>(mBossGenerator_.get());
#pragma endregion

#pragma region コライダー
	colBossGenerator_ = std::make_unique<SphereCollider>(float3{0.0f, 3.0f, 0.0f});
	colBossGenerator_->SetAttribute(COL_BOSSGENERATOR);
	colBossGenerator_->SetObject3D(oBossGenerator_.get());
	colMgr_->AddCollider(colBossGenerator_.get());
#pragma endregion
}

void EnemyManager::Update()
{
	if (boss_) boss_->Update();
}

void EnemyManager::Draw()
{
	oBossGenerator_->Draw();
	if (boss_) {
		boss_->Draw();
	}
}

void EnemyManager::ObjUpdate()
{
	oBossGenerator_->Update();
	if (boss_) {
		boss_->ObjUpdate();
	}
}

void EnemyManager::OnCollision()
{
	if (boss_) boss_->OnCollision();
}

void EnemyManager::SummonBoss()
{
	boss_ = std::make_unique<Boss0>();
	boss_->Initialize(oBossGenerator_->GetPosition() + float3{0.0f, 3.0f, 0.0f});
	boss_->ObjUpdate();
}

void EnemyManager::SetBossGenerator(const float3& pos)
{
	oBossGenerator_->SetPosition(pos);
}