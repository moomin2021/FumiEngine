#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"

// インスタンス取得
SceneManager* SceneManager::GetInstance()
{
	// 関数内static変数として宣言
	static SceneManager instance;
	return &instance;
}

void SceneManager::ChangeScene(int sceneNo)
{
	// 現在のシーンが空ではなかったら
	//if (nowScene_ != nullptr) {
	//	// 現在のシーンをメモリ解放
	//	delete nowScene_;
	//}
	delete nowScene_;

	// 指定されたシーンのインスタンス生成と初期化
	switch (sceneNo)
	{
	case SCENE::TITLE:
		nowScene_ = new TitleScene();
		break;
	case SCENE::GAME:
		nowScene_ = new GameScene();// -> インスタンス生成
		break;
	}
	nowScene_->Initialize();
}

// コンストラクタ
SceneManager::SceneManager() {
	// インスタンス取得
	key_ = Key::GetInstance();

	// 最初のシーン
	nowScene_ = new GameScene();
	nowScene_->Initialize();
}

// デストラクタ
SceneManager::~SceneManager() {
	delete nowScene_;
}

// 更新処理
void SceneManager::Update() {
	if (key_->TriggerKey(DIK_1)) {
		ChangeScene(SCENE::TITLE);
	}
	if (key_->TriggerKey(DIK_2)) ChangeScene(SCENE::GAME);

	nowScene_->Update();
}

// 描画処理
void SceneManager::Draw()
{
	nowScene_->Draw();
}
