#pragma once
#include "Vector3.h"
#include "Camera.h"

class Player {
	// メンバ変数
public:

private:
	// 足元の座標
	Vector3 legPos_;

	// カメラ
	Camera* camera_;

	// メンバ関数
public:
	// コンストラクタ
	Player();

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
	// デストラクタ
	~Player();

};