#pragma once
#include "Camera.h"

#include <memory>

class MapCreatePlayer
{
private:
	enum class State {
		NORMAL,
		AIR,
	};

#pragma region メンバ変数
private:
	// カメラ
	Camera* camera_ = nullptr;

	// 状態
	State state_ = State::NORMAL;

	// 重力
	float yVel_ = 0.0f;				// y方向の速度
	const float gravityAcc_ = -0.1f;// 重力加速度
	const float jumpSpd_ = 1.0f;	// ジャンプ速度
#pragma endregion

#pragma region メンバ関数
public:
	MapCreatePlayer() {}
	~MapCreatePlayer();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	// デバック処理
	void Debug();

private:
	// 状態別処理
	static void (MapCreatePlayer::* stateTable[]) ();
	void Normal();	// 通常状態
	void Fly();		// 飛翔状態

	void Fall();	// 落下処理
#pragma endregion

#pragma region セッター関数
public:
	void SetCamera(Camera* pCamera) { camera_ = pCamera; }
#pragma endregion
};
