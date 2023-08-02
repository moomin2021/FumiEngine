#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Sound.h"
#include "ParticleManager.h"
#include "Camera.h"

#include <memory>

class Scene1 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_ = nullptr;

	std::unique_ptr<Camera> camera_ = nullptr;

	// パーティクル
	std::unique_ptr<ParticleManager> particle_ = nullptr;

	// パーティクル画像
	uint16_t particleHandle_ = 0;

	// サウンド
	uint16_t soundKey_;

	// --メンバ関数-- //
public:
	// コンストラクタ
	Scene1();

	// デストラクタ
	~Scene1();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
};

