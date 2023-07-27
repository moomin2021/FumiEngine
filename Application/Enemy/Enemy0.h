#pragma once
#include "BaseEnemy.h"
#include "Bullet.h"
#include "Object3D.h"
#include "SphereCollider.h"
#include "Player.h"

#include <memory>

class Enemy0 : public BaseEnemy
{
	enum State {
		WAIT,		// 待機状態
		RANDOMMOVE,	// ランダム移動状態
		CHASE,		// 追跡状態
	};

#pragma region メンバ変数
private:
	// プライヤー
	static Player* player_;

	// 状態
	State state_ = WAIT;

	// 弾
	std::vector<std::unique_ptr<Bullet>> bullets_;
	static Model* sBulletModel_;

	// 各状態の開始時間
	uint64_t waitStartTime_ = 0;
	uint64_t rndMoveStartTime_ = 0;

	// 各状態の制限時間
	uint16_t waitTime_ = 3;
	uint16_t rndMoveTime_ = 5;

	// 横移動切り替え時間
	uint16_t horizontalMoveSwitchTime_ = 3;

	// 横移動開始時間
	uint64_t horizontalMoveStartTime_ = 0;

	// 横移動速度
	float horizontalMoveSpd_ = 0.3f;

	// 前後移動速度
	float frontRearMoveSpd_ = 0.1f;

	// 右移動か
	float isMoveRight_ = 1.0f;

	// ランダム移動時の移動方向
	Vector3 randomMoveVec_ = {};

	// 索敵範囲
	float searchRange_ = 100.0f;

	// ランダム移動時の速度
	float rndMoveSpd_ = 0.1f;

	// 撃つ間隔
	uint16_t shootInterval_ = 3;

	// 撃った時間
	uint64_t shootTime_ = 0;

	// HP
	uint16_t hp_;

	uint16_t damageCounter_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model"> モデル </param>
	Enemy0(Model* model);

	// デストラクタ
	~Enemy0();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos"> 初期位置 </param>
	void Initialize(float3 pos, float3 scale);

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

private:
	// 状態別処理
	static void (Enemy0::* stateTable[]) ();
	void Wait();		// 待機状態
	void RandomMove();	// ランダム移動状態
	void Chase();		// 追跡状態

	// 弾を撃つ
	void Shoot();

	// 衝突時の処理
	void OnCollision();
#pragma endregion

#pragma region セッター関数
public:
	/// <summary>
	/// プレイヤーを設定
	/// </summary>
	static void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 弾のモデルを設定
	/// </summary>
	static void SetBulletModel(Model* model) { sBulletModel_ = model; }
#pragma endregion
};