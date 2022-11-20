#include "SceneManager.h"

// 現在のシーンを空にする
BaseScene* SceneManager::myScene = nullptr;

void SceneManager::ChangeScene(SCENE scene) {
	// 現在のシーンが空ではなかったら
	if (myScene != nullptr) {
		delete myScene;// -> 現在のシーンをメモリ解放処理
	}

	switch (scene)
	{
	case SCENE::SCENE1:
		break;
	default:
		break;
	}
}

void SceneManager::Update() {
	// 現在のシーンの更新処理
	myScene->Update();
}

void SceneManager::Draw() {
	// 現在のシーンの描画処理
	myScene->Draw();
}