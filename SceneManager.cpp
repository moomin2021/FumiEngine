#include "SceneManager.h"
#include "Scene1.h"

// 現在のシーンを空にする
BaseScene* SceneManager::myScene_ = nullptr;

SceneManager::SceneManager() {
	// インスタンス取得
	key_ = Key::GetInstance();
}

SceneManager::~SceneManager() {
	// 現在のシーンが空ではなかったら
	if (myScene_ != nullptr) {
		delete myScene_;// -> 現在のシーンをメモリ解放処理
	}
}

void SceneManager::ChangeScene(SCENE scene) {
	// 現在のシーンが空ではなかったら
	if (myScene_ != nullptr) {
		delete myScene_;// -> 現在のシーンをメモリ解放処理
	}

	switch (scene)
	{
	// シーン1に設定
	case SCENE::SCENE1:
		myScene_ = new Scene1();
		myScene_->Initialize();
		break;
	default:
		break;
	}
}

void SceneManager::Update() {
	// 現在のシーンの更新処理
	myScene_->Update();
}

void SceneManager::Draw() {
	// 現在のシーンの描画処理
	myScene_->Draw();
}