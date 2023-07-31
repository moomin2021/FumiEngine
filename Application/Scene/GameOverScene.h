#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "Sprite.h"
#include "Key.h"

#include <memory>

class GameOverScene : public BaseScene
{
#pragma region メンバ変数
private:
	// キーボード入力
	Key* key_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sGameOver_ = nullptr;

	// テクスチャ
	uint16_t gameOverHandle_ = 0;

#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	GameOverScene();

	// デストラクタ
	~GameOverScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
#pragma endregion
};