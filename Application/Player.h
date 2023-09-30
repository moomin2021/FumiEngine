#pragma once
#include "Key.h"
#include "Mouse.h"
#include "CollisionManager.h"
#include "RayCollider.h"

#include "Camera.h"

#include <string>
#include <vector>
#include <memory>

class Player
{
	enum State {
		NORMAL,	// 通常状態
		AIR,	// 空中状態
		CLIMB,	// 登り状態
	};

	std::vector<std::string> stateName_ = {
		"NORMAL",
		"AIR",
		"CLIMB"
	};

#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;
	CollisionManager* colMgr_ = nullptr;

	// 状態
	State state_ = NORMAL;

	// カメラ関連
	std::unique_ptr<Camera> camera_ = nullptr;// カメラ本体
	float sencivity_ = 0.1f;// カメラ感度
	Vector3 eyeAngle_	= { 0.0f, 0.0f, 0.0f };// カメラ角度
	Vector3 forwardVec_	= { 0.0f, 0.0f, 0.0f };// 正面ベクトル
	Vector3 rightVec_	= { 0.0f, 0.0f, 0.0f };// 右ベクトル

	// モデル
	std::unique_ptr<Model> mSphere_ = nullptr;// 球

	// オブジェクト
	std::unique_ptr<Object3D> oCol_ = nullptr;// コライダーの基準となるオブジェクト3D

	// コライダー
	std::unique_ptr<RayCollider> legCol_ = nullptr;// 足元コライダー

	// 移動関連
	float moveSpd_ = 0.0f;// 移動速度
	float maxMoveSpd_ = 0.5f;// 最大移動速度
	float moveAcc_ = 0.05f;// 移動加速度
	bool isDash_ = false;// ダッシュフラグ
	float dashSpd_ = 1.0f;// ダッシュ速度

	// ジャンプ関連
	float gravity_ = 0.0f;// 重力
	float maxGravity_ = 1.5f;// 最大重力
	float gAcc_ = 0.2f;// 重力加速度
	float jumpSpd_ = 1.5f;// ジャンプ速度

#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 3D描画処理
	void Draw3D();

	// 衝突判定時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

private:
	// 状態別処理
	static void (Player::* stateTable[]) ();
	void Normal();	// 通常状態
	void Air();		// 空中状態
	void Climb();	// 登り状態

	// 行動関数
	void Move();	// 移動操作
	void EyeMove();	// 視点操作
	void Jump();	// ジャンプ処理
	void Fall();	// 落下処理
#pragma endregion
};