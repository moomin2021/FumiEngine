#include "SceneManager.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"

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
	case SCENE::SCENE2:
		nowScene_ = new Scene2();
		nowScene_->Initialize();
		break;
	case SCENE::SCENE3:
		nowScene_ = new Scene3();
		nowScene_->Initialize();
		break;
	case SCENE::SCENE4:
		nowScene_ = new Scene4();
		nowScene_->Initialize();
		break;
	}
}

// 更新処理
void SceneManager::Update() {

	if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE::SCENE1);
	if (key_->TriggerKey(DIK_2)) ChangeScene(SCENE::SCENE2);
	if (key_->TriggerKey(DIK_3)) ChangeScene(SCENE::SCENE3);
	if (key_->TriggerKey(DIK_4)) ChangeScene(SCENE::SCENE4);

	nowScene_->Update();
}

// 描画処理
void SceneManager::Draw()
{
	nowScene_->Draw();
}
