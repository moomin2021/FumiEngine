#include "GameScene.h"
#include "CollisionManager.h"

#include "PipelineManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize()
{
#pragma region ライトグループ
	lightGroup_ = std::make_unique<LightGroup>();
	Object3D::SetLightGroup(lightGroup_.get());

	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region ステージクラス
	stage_ = std::make_unique<Stage>();
	stage_->Initialize();
	stage_->Load("Resources/StageJson/stage1.json");
#pragma endregion

#pragma region プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
#pragma endregion
}

void GameScene::Update()
{
	// プレイヤー
	player_->Update();

	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void GameScene::Draw()
{
	// ステージクラス
	stage_->Draw();

	// プレイヤー
	player_->Draw3D();
}

void GameScene::OnCollision()
{
	// 衝突判定をとる
	CollisionManager::GetInstance()->CheckAllCollision();

	// プレイヤー
	player_->OnCollision();
}

void GameScene::MatUpdate()
{
	// ステージクラス
	stage_->MatUpdate();

	// プレイヤー
	player_->MatUpdate();
}