#include "GameOverScene.h"

#include "PipelineManager.h"
#include "WinAPI.h"
#include "Texture.h"

GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {}

void GameOverScene::Initialize()
{
	Vector2 winSize = 
	{	(float)WinAPI::GetInstance()->GetWidth(),
		(float)WinAPI::GetInstance()->GetHeight() };

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 0.0f, -10.0f });
	Sprite::SetCamera(camera_.get());
#pragma endregion

#pragma region スプライト
	resultFrameS_ = std::make_unique<Sprite>();
	resultFrameS_->SetAnchorPoint({ 0.5f, 0.5f });
	resultFrameS_->SetPosition({ winSize.x / 2.0f, winSize.y / 2.0f });
	resultFrameS_->SetSize({ 1280.0f, 720.0f });
#pragma endregion

#pragma region 画像
	resultFrameH_ = LoadTexture("Sprite/resultFrame.png");
#pragma endregion
}

void GameOverScene::Update()
{
	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void GameOverScene::Draw()
{
	PipelineManager::PreDraw("Sprite");

	resultFrameS_->Draw(resultFrameH_);
}

void GameOverScene::OnCollision()
{
}

void GameOverScene::MatUpdate()
{
	// カメラ
	camera_->Update();

	resultFrameS_->MatUpdate();
}
