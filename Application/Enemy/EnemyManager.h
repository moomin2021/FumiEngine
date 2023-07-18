#pragma once
#include "BaseEnemy.h"

#include <memory>
#include <list>

class EnemyManager {
#pragma region メンバ変数
	// 敵
	std::list<std::unique_ptr<BaseEnemy>> enemys_;
#pragma endregion

#pragma region メンバ関数
public:
	// インスタンス取得
	static EnemyManager* GetInstance();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	/// <summary>
	/// 敵追加
	/// </summary>
	/// <param name="enemy"> 敵 </param>
	void AddEnemy(BaseEnemy* enemy) { enemys_.emplace_back(enemy); }

private:
	// コンストラクタ
	EnemyManager();
#pragma endregion

#pragma region 特殊関数
	EnemyManager(const EnemyManager&) = delete;				// コピーコンストラクタ禁止
	EnemyManager& operator = (const EnemyManager&) = delete;// コピー代入演算子禁止
#pragma endregion
};