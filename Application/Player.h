#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Camera.h"
#include "Bullet.h"
#include "Sprite.h"
#include "RayCollider.h"

#include <memory>

class Player
{
#pragma region メンバ変数
public:
	// 弾のクラス
	std::vector <std::unique_ptr<Bullet>> bullets_;

private:
	// 入力クラスインスタンス
	Key* key_;		// キーボード
	Mouse* mouse_;	// マウス

	// 視点カメラ
	std::unique_ptr<Camera> camera_;

	// 弾のモデル
	std::unique_ptr<Model> mBullet_;

	// レイのコライダー
	std::unique_ptr<RayCollider> eyeCollider_;// 視点のレイ
	std::unique_ptr<RayCollider> downCollider_;// 真下に向いている地面検出用のレイ

	// 方向ベクトル
	Vector3 forwardVec_;// 前方
	Vector3 rightVec_;	// 右

	// 速度
	float moveAcc_;			// 移動加速度
	float maxSpd_;// 最大速度
	float moveSpd_;
	float cameraAngleSpd_;	// カメラの角度の移動速度

	// クロスヘア
	uint16_t crossHairHandle_;
	std::unique_ptr<Sprite> sCrossHair_;

	// ジャンプ用
	bool isGround_;// 地面についているか
	float gravity_;// 重力
	float gAcc_;// 重力加速度
	float jumpSpd_;// ジャンプ速度
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// プレイヤー
	/// </summary>
	Player();

	// デストラクタ
	~Player();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 2D描画処理
	/// </summary>
	void Draw2D();

private:
	// 弾を撃つ処理
	void Shoot();

	/// <summary>
	/// 視点移動処理
	/// </summary>
	void EyeMove();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	// コライダーの更新
	void ColliderUpdate();

	// 衝突時の処理
	void OnCollision();
#pragma endregion

#pragma region ゲッター関数

#pragma endregion
};