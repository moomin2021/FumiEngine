#pragma once
#include "BaseScene.h"

class SceneManager {
public:
	// シーン列挙型
	enum SCENE {
		SCENE1,
	};

	// シーンを引数のシーンに変える
	static void ChangeScene(SCENE scene);

	// 現在のシーンの更新処理
	static void Update();

	// 現在のシーンの描画処理
	static void Draw();

private:
	// 現在のシーン
	static BaseScene* myScene;
};