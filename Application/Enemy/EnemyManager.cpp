#include "EnemyManager.h"

EnemyManager* EnemyManager::GetInstance()
{
	// インスタンス生成
	static EnemyManager inst;

	// インスタンスを返す
	return &inst;
}

void EnemyManager::Update()
{
	// 敵更新処理
	std::list<std::unique_ptr<BaseEnemy>>::iterator it;
	for (it = enemys_.begin(); it != enemys_.end(); it++) {
		(*it)->Update();
	}

	//for (size_t i = 0; i < enemys_.size(); i++) {
	//	enemys_[i]->Update();
	//}
}

void EnemyManager::Draw()
{
	// 敵描画処理
	std::list<std::unique_ptr<BaseEnemy>>::iterator it;
	for (it = enemys_.begin(); it != enemys_.end(); it++) {
		(*it)->Draw();
	}

	//for (size_t i = 0; i < enemys_.size(); i++) {
	//	enemys_[i]->Draw();
	//}
}

EnemyManager::EnemyManager() {

}