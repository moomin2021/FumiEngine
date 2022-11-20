#pragma once
#include "BaseScene.h"
#include "Key.h"
#include <memory>

class SceneManager {
	// --メンバ関数-- //
public:
	// シーン列挙型
	enum SCENE {
		SCENE1,
	};

	// コンストラクタ
	SceneManager();

	// デストラクタ
	~SceneManager();

	// シーンを引数のシーンに変える
	static void ChangeScene(SCENE scene);

	// 現在のシーンの更新処理
	static void Update();

	// 現在のシーンの描画処理
	static void Draw();

private:

	// --メンバ変数-- //
public:

private:
	// 現在のシーン
	static BaseScene* myScene_;

	// キーボード入力
	Key* key_;
};