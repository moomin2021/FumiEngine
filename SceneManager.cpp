#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "ClearScene.h"

BaseScene* SceneManager::nowScene_ = nullptr;

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
	nowScene_ = new TitleScene();
	nowScene_->Initialize();
}

// デストラクタ
SceneManager::~SceneManager() {
	delete nowScene_;
}

void SceneManager::ChangeScene(int changeSceneNum)
{
	// 現在のシーンを解放処理
	if (nowScene_ != nullptr) {
		delete nowScene_;
	}

	switch (changeSceneNum)
	{
	case SCENE::TITLE:
		nowScene_ = new TitleScene();
		nowScene_->Initialize();
		break;
	case SCENE::GAME:
		nowScene_ = new GameScene();
		nowScene_->Initialize();
		break;
	case SCENE::CLEAR:
		nowScene_ = new ClearScene();
		nowScene_->Initialize();
		break;
	}
}

// 更新処理
void SceneManager::Update() {

	if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE::TITLE);
	if (key_->TriggerKey(DIK_2)) ChangeScene(SCENE::GAME);
	if (key_->TriggerKey(DIK_3)) ChangeScene(SCENE::CLEAR);

	nowScene_->Update();
}

// 描画処理
void SceneManager::Draw()
{
	nowScene_->Draw();
}
