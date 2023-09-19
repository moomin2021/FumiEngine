#pragma once
#include "BaseScene.h"
#include "Camera.h"

#include <memory>

class TestScene : public BaseScene
{
#pragma region メンバ変数
private:
	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	TestScene();

	// デストラクタ
	~TestScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion
};