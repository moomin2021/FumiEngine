#include "GameScene.h"
#include "Texture.h"
#include "PipelineManager.h"

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
#pragma region インスタンス
	key_ = Key::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye(Vector3(0.0f, 0.0f, -10.0f));
	Sprite::SetCamera(camera_.get());
#pragma endregion

#pragma region スプライト
	sprite_ = std::make_unique<Sprite>();
	handle_ = LoadTexture("Sprite/hae.png");
#pragma endregion
}

void GameScene::Update()
{
	MatUpdate();
	OnCollision();
}

void GameScene::Draw()
{
	PipelineManager::PreDraw("Sprite");
	sprite_->Draw(handle_);
}

void GameScene::MatUpdate()
{
	camera_->Update();
	sprite_->MatUpdate();
}

void GameScene::OnCollision()
{

}