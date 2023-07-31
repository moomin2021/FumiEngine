#pragma once
#include "BaseEnemy.h"
#include "Player.h"

#include <memory>
#include <vector>
#include <list>
#include <vector>

class EnemyManager {
#pragma region メンバ変数
	// エネミー配列
	std::list<std::unique_ptr<BaseEnemy>> enemys_;
	static std::vector<std::unique_ptr<Bullet>> bullets_;

	// 敵モデル
	std::unique_ptr<Model> mEnemy0_ = nullptr;
	static std::unique_ptr<Model> mEnemy0Bullet_;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	EnemyManager();

	// デストラクタ
	~EnemyManager();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// エネミーを生成追加
	void CreateAddEnemy0(const float3& pos, const float3& scale);

	// 弾を追加
	static void AddBullet(BulletType type, const float3& iniPos, const Vector3& moveVec);
#pragma endregion

#pragma region セッター関数
public:
	// プレイヤーを設定
	void SetPlayer(Player* player);
#pragma endregion

#pragma region ゲッター関数

#pragma endregion

#pragma region 特殊関数
	EnemyManager(const EnemyManager&) = delete;				// コピーコンストラクタ禁止
	EnemyManager& operator = (const EnemyManager&) = delete;// コピー代入演算子禁止
#pragma endregion
};