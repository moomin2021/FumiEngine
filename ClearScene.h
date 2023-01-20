#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Key.h"
#include "Sprite.h"
#include <memory>

class ClearScene : public BaseScene {
	// --メンバ変数-- //
public:

private:
	// キーボード入力
	Key* key_;

	// カメラ
	Camera* camera_;

	std::unique_ptr<Sprite> clearS_;
	int clearH_;

	// --メンバ関数-- //
public:
	// コンストラクタ
	ClearScene();

	// デストラクタ
	~ClearScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:

};