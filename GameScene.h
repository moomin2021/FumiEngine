#pragma once
#include "BaseScene.h"// -> シーンのベース
#include "Sprite.h"// -> スプライト
#include "Key.h"// -> キーボード入力

class GameScene : public BaseScene {
	// --メンバ変数-- //
public:

private:
	// キーボード入力クラス
	Key* key_;

	// スプライト
	Sprite* sprite_;

	// 描画フラグ
	bool isDraw_;

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