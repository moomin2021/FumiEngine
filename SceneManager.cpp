#include "SceneManager.h"
#include "Scene1.h"
#include "Scene2.h"

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
	case SCENE::SCENE2:
		myScene_ = new Scene2();
		myScene_->Initialize();
	default:
		break;
	}
}

void SceneManager::Update() {
	// 現在のシーンの更新処理
	myScene_->Update();

	if (key_->TriggerKey(DIK_1)) {
		ChangeScene(SCENE1);
	}

	else if (key_->TriggerKey(DIK_2)) {
		ChangeScene(SCENE2);
	}
}

void SceneManager::Draw() {
	// 現在のシーンの描画処理
	myScene_->Draw();
}