#pragma once
#include "Vector3.h"
#include "Camera.h"
#include "Key.h"
#include "Mouse.h"
#include "Object3D.h"
#include "ColType.h"
#include "Sprite.h"

class Player {
	// メンバ変数
public:
	Circle2D col_;// -> 当たり判定用データ
	Circle2D oldCol_;

	Line3D ray_;
	float rayDist_;

private:
	Key* key_;// -> キーボード入力
	Mouse* mouse_;// -> マウス入力
	Camera* camera_;// -> カメラ

	float angleX_, angleY_;// -> カメラアングル
	float cameraSens_;// -> カメラ感度
	float moveSpeed_;// -> 移動速度

	Vector3 forwardVec_;// -> 前方ベクトル
	Vector3 rightVec_;// -> 右ベクトル

	int reticleH_;// -> レティクルの画像
	Sprite* reticle_;// -> レティクルのスプライト

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

	void Target();

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