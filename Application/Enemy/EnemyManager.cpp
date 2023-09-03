#include "EnemyManager.h"
#include "CollisionAttribute.h"
#include "SceneManager.h"

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
	mEnemy0_ = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region オブジェクト
	oBossGenerator_ = std::make_unique<Object3D>(mBossGenerator_.get());
#pragma endregion

#pragma region コライダー
	colBossGenerator_ = std::make_unique<SphereCollider>(Vector3{ 0.0f, 3.0f, 0.0f });
	colBossGenerator_->SetAttribute(COL_BOSSGENERATOR);
	colBossGenerator_->SetObject3D(oBossGenerator_.get());
	colMgr_->AddCollider(colBossGenerator_.get());
#pragma endregion
}

void EnemyManager::Update()
{
	for (auto it = enemys_.begin(); it != enemys_.end();) {
		// 敵の更新
		(*it)->Update();

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false) it = enemys_.erase(it);
		else ++it;
	}

	if (boss_) {
		boss_->Update();
	}
}

void EnemyManager::Draw()
{
	oBossGenerator_->Draw();
	if (boss_) {
		boss_->Draw();
	}

	// 敵描画処理
	for (auto& i : enemys_) {
		i->Draw();
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
	boss_->Initialize(oBossGenerator_->GetPosition() + Vector3{0.0f, 3.0f, 0.0f});
	boss_->ObjUpdate();
}

void EnemyManager::CreateAddEnemy0(const Vector3& pos, const Vector3& scale)
{
	// 敵の生成
	std::unique_ptr<Enemy0> newEnemy = std::make_unique<Enemy0>(mEnemy0_.get());
	newEnemy->Initialize(pos, scale);

	// エネミー配列に追加
	enemys_.emplace_back(std::move(newEnemy));
}

void EnemyManager::CheckSceneChange()
{
	if (boss_ && boss_->GetIsAlive() == false) {
		SceneManager::GetInstance()->SceneTransition(SCENE::TITLE);
	}
}

void EnemyManager::SetBossGenerator(const Vector3& pos)
{
	oBossGenerator_->SetPosition(pos);
}

void EnemyManager::SetPlayer(Player* player)
{
	Enemy0::SetPlayer(player);
}