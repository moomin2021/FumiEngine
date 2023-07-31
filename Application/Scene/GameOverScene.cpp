#include "GameOverScene.h"
#include "PipelineManager.h"
#include "SceneManager.h"
#include "Texture.h"

GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {}

void GameOverScene::Initialize()
{
	// インスタンス取得
	key_ = Key::GetInstance();

	// カメラ設定
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 0.0f, -5.0f });

	// スプライト生成
	sGameOver_ = std::make_unique<Sprite>();
	sGameOver_->SetSize({ 1920.0f, 1080.0f });

	// 画像読み込み
	gameOverHandle_ = LoadTexture("Resources/GameOver.png");
}

void GameOverScene::Update()
{
	// カメラ更新
	camera_->Update();

	if (key_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene(SCENE::TITLE);
	}
}

void GameOverScene::Draw()
{
	PipelineManager::PreDraw("Sprite");

	// スプライト描画
	sGameOver_->Draw(gameOverHandle_);
}