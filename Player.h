#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Camera.h"

#include <memory>

class Player
{
#pragma region メンバ変数
private:
	// 入力クラスインスタンス
	Key* key_;		// キーボード
	Mouse* mouse_;	// マウス

	// 視点カメラ
	std::unique_ptr<Camera> camera_;

	// 方向ベクトル
	Vector3 forwardVec_;// 前方
	Vector3 rightVec_;	// 右

	// 速度
	float moveSpd_;			// 移動速度
	float cameraAngleSpd_;	// カメラの角度の移動速度
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// プレイヤー
	/// </summary>
	Player();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// 視点移動処理
	/// </summary>
	void EyeMove();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
#pragma endregion
};