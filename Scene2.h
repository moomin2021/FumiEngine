#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Sprite.h"
#include "Key.h"
#include <memory>

class Scene2 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_;

	// カメラ
	std::unique_ptr<Camera> camera_;

	// スプライト
	std::unique_ptr<Sprite> sprite_[2];

	// テクスチャハンドル
	int texHandle_[2];

	// --メンバ関数-- //
public:
	// コンストラクタ
	Scene2();

	// デストラクタ
	~Scene2();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
};

