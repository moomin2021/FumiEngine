#pragma once
#include "Camera.h"
#include "Player.h"

#include <memory>

class DebugCamera
{
#pragma region メンバ変数
private:
	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// 情報
	Vector3 eye_ = Vector3(0.0f, 10.0f, 0.0f);
	Vector3 target_ = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 up_ = Vector3(0.0f, 1.0f, 0.0f);

	// プレイヤー
	Player* pPlayer_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化処理
	void Initialize(Player* inPlayer);

	// 更新処理
	void Update();

	// 行列更新処理
	void MatUpdate();

	void Debug();
#pragma endregion

#pragma region ゲッター関数
	Camera* GetCamera() { return camera_.get(); }
#pragma endregion
};