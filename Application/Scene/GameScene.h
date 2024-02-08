#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Camera.h"
#include "Sprite.h"

#include <memory>

class GameScene : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;
	uint16_t handle_ = 0;

	// --メンバ関数-- //
public:
	// コンストラクタ
	GameScene() {}

	// デストラクタ
	~GameScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 行列更新処理
	void MatUpdate();

	// 衝突時処理
	void OnCollision();
};

