#pragma once
#include "Vector3.h"
#include "Camera.h"
#include "Key.h"
#include "Mouse.h"

class Player {
	// メンバ変数
public:
	Vector3 oldPos_;// -> 前フレームの座標

private:
	Key* key_;// -> キーボード入力
	Mouse* mouse_;// -> マウス入力
	Camera* camera_;// -> カメラ

	// カメラアングル
	float angleX_, angleY_;

	// カメラ感度
	float cameraSens_;

	float moveSpeed_;// -> 移動速度

	Vector3 forwardVec_;// -> 前方ベクトル
	Vector3 rightVec_;// -> 右ベクトル

	// メンバ関数
public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

#pragma region セッター
	// カメラを設定
	void SetCamera(Camera* camera) { camera_ = camera; }
#pragma endregion

private:
	// 視点移動
	void EyeMove();

	// 移動処理
	void Move();
};