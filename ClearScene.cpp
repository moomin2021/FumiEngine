#include "ClearScene.h"
#include "FumiEngine.h"
#include "SceneManager.h"

ClearScene::ClearScene() {

}

ClearScene::~ClearScene() {

}

void ClearScene::Initialize() {
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	clearS_ = std::make_unique<Sprite>();
	clearS_->scale = { 19.20f, 10.80f };
	clearS_->Update();

	clearH_ = LoadTexture("Resources/ClearImage.png");
}

void ClearScene::Update() {
	clearS_->Update();

	if (key_->TriggerKey(DIK_SPACE)) {
		SceneManager::ChangeScene(SCENE::TITLE);
	}
}

void ClearScene::Draw() {
	Sprite::PreDraw();
	clearS_->Draw(clearH_);
}