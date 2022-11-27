#pragma once
#include "BaseScene.h"// -> シーンのベース
#include "Sprite.h"// -> スプライト

class GameScene : public BaseScene {
	// --メンバ変数-- //
public:

private:
	// スプライト
	Sprite* sprite_;

	// --メンバ関数-- //
public:
	// コンストラクタ
	GameScene();

	// デストラクタ
	~GameScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:

};