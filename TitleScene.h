#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Key.h"
#include "Sprite.h"
#include <memory>

class TitleScene : public BaseScene {
	// --メンバ変数-- //
public:

private:
	// キーボード入力
	Key* key_;

	std::unique_ptr<Sprite> titleS_;
	int titleImage_;

	std::unique_ptr<Sprite> spaceStartText_;
	int spaceStartTextH_;

	// カメラ
	Camera* camera_;

	// --メンバ関数-- //
public:
	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:

};