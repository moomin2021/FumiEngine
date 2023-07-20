#pragma once
#include "BaseEnemy.h"

#include <memory>
#include <vector>
#include <list>

class EnemyManager {
#pragma region メンバ変数
	// エネミー配列
	std::list<std::unique_ptr<BaseEnemy>> enemys_;

	// 敵モデル
	std::unique_ptr<Model> mEnemy0_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	EnemyManager();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// エネミーを生成追加
	void CreateAddEnemy0(const float3& pos, const float3& scale);
#pragma endregion

#pragma region セッター関数
	
#pragma endregion

#pragma region ゲッター関数

#pragma endregion

#pragma region 特殊関数
	EnemyManager(const EnemyManager&) = delete;				// コピーコンストラクタ禁止
	EnemyManager& operator = (const EnemyManager&) = delete;// コピー代入演算子禁止
#pragma endregion
};