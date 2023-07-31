#include "EnemyManager.h"
#include "CollisionManager.h"
#include "Enemy0.h"

std::unique_ptr<Model> EnemyManager::mEnemy0Bullet_ = nullptr;
std::vector<std::unique_ptr<Bullet>> EnemyManager::bullets_ = {};

EnemyManager::EnemyManager() {
	// モデル読み込み
	mEnemy0_ = std::make_unique<Model>("sphere");
	mEnemy0Bullet_ = std::make_unique<Model>("sphere");
}

void EnemyManager::Update()
{
	// 敵更新処理
	std::list<std::unique_ptr<BaseEnemy>>::iterator it;
	for (it = enemys_.begin(); it != enemys_.end(); it++) {
		(*it)->Update();
	}

	for (size_t i = 0; i < bullets_.size(); i++) {
		// 弾の更新処理
		bullets_[i]->Update();

		// 生存フラグが[OFF]だったら
		if (bullets_[i]->GetIsAlive() == false) {
			// 弾を消す
			bullets_.erase(bullets_.begin() + i);
		}
	}
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Draw()
{
	// 敵描画処理
	std::list<std::unique_ptr<BaseEnemy>>::iterator it;
	for (it = enemys_.begin(); it != enemys_.end(); it++) {
		(*it)->Draw();
	}

	for (auto& i : bullets_) {
		i->Draw();
	}
}

void EnemyManager::CreateAddEnemy0(const float3& pos, const float3& scale)
{
	// 敵の生成
	std::unique_ptr<Enemy0> newEnemy = std::make_unique<Enemy0>(mEnemy0_.get());
	newEnemy->Initialize(pos, scale);

	// エネミー配列に追加
	enemys_.emplace_back(std::move(newEnemy));
}

void EnemyManager::AddBullet(BulletType type, const float3& iniPos, const Vector3& moveVec)
{
	bullets_.emplace_back(std::make_unique<Bullet>(mEnemy0Bullet_.get(), ENEMY0, iniPos, moveVec));
}

void EnemyManager::SetPlayer(Player* player)
{
	Enemy0::SetPlayer(player);
}