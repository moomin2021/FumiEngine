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
	Key* key_ = nullptr;		// キーボード
	Mouse* mouse_ = nullptr;	// マウス

	// 視点カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// 弾のモデル
	std::unique_ptr<Model> mBullet_ = nullptr;

	// レイのコライダー
	std::unique_ptr<RayCollider> eyeCollider_ = nullptr;// 視点のレイ

	// 方向ベクトル
	Vector3 forwardVec_ = {};// 前方
	Vector3 rightVec_ = {};// 右

	// 速度
	float moveAcc_ = 0.1f;			// 移動加速度
	float maxSpd_ = 1.5f;			// 最大速度
	float moveSpd_ = 0.0f;			// 移動速度
	float cameraAngleSpd_ = 0.1f;	// カメラの角度の移動速度

	// クロスヘア
	uint16_t crossHairHandle_ = 0;
	std::unique_ptr<Sprite> sCrossHair_ = nullptr;

	// ジャンプ用
	bool isGround_ = false;	// 地面についているか
	float gravity_ = 0.0f;	// 重力
	float gAcc_ = 0.2f;		// 重力加速度
	float jumpSpd_ = 2.5f;	// ジャンプ速度
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 更新処理
	void Update();

	// オブジェクト3D描画処理
	void Object3DDraw();

	// 前面スプライト描画
	void FrontSpriteDraw();

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