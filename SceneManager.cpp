#include "SceneManager.h"
#include "GameScene.h"

// インスタンス取得
SceneManager* SceneManager::GetInstance()
{
	// 関数内static変数として宣言
	static SceneManager instance;
	return &instance;
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
	nowScene_->Update();
}

// 描画処理
void SceneManager::Draw()
{
	nowScene_->Draw();
}
