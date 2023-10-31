#pragma once
#include "Camera.h"

#include <memory>

class MapCreatePlayer
{
#pragma region メンバ変数
private:
	// カメラ
	Camera* camera_ = nullptr;
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
#pragma endregion

#pragma region セッター関数
	void SetCamera(Camera* pCamera) { camera_ = pCamera; }
#pragma endregion
};
