#include "SceneManager.h"
<<<<<<< HEAD
#include "GameScene.h"
#include "TitleScene.h"

BaseScene* SceneManager::nowScene_ = nullptr;
=======
#include "Scene1.h"
>>>>>>> master

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
	nowScene_ = new Scene1();
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
	case SCENE::SCENE1:
		nowScene_ = new Scene1();
		nowScene_->Initialize();
		break;
	}
}

// 更新処理
void SceneManager::Update() {

<<<<<<< HEAD
	if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE::TITLE);
	if (key_->TriggerKey(DIK_2)) ChangeScene(SCENE::GAME);
<<<<<<< HEAD
	if (key_->TriggerKey(DIK_3)) ChangeScene(SCENE::CLEAR);
=======
	if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE::SCENE1);
>>>>>>> master
=======
>>>>>>> parent of fcb6802 (邨ゆｺ�)

	nowScene_->Update();
}

// 描画処理
void SceneManager::Draw()
{
	nowScene_->Draw();
}
