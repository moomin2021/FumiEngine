#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Camera.h"
#include "Player.h"

#include <memory>

class DebugCamera
{
#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// 情報
	Vector3 eye_ = Vector3(0.0f, 15.0f, 0.0f);
	Vector3 target_ = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 up_ = Vector3(0.0f, 1.0f, 0.0f);

	float moveSpd_ = 0.1f;
	float minEyeY_ = 10.0f;
	float maxEyeY_ = 50.0f;

	// プレイヤー
	Player* pPlayer_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	void Initialize();
	void Update();
	void MatUpdate();
	void Debug(bool isDebug);

private:
	void Move();
	void Zoom();
#pragma endregion

#pragma region ゲッター関数
public:
	Camera* GetCamera() { return camera_.get(); }
#pragma endregion
};