#include "GameOverScene.h"

#include "PipelineManager.h"

GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {}

void GameOverScene::Initialize()
{
#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 0.0f, -10.0f });
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ライトグループ
	lightGroup_ = LightGroup::GetInstance();
	Object3D::SetLightGroup(lightGroup_);

	dirLight_ = std::make_unique<DirectionalLight>();
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region モデル
	model_ = std::make_unique<Model>("cube");
#pragma endregion

#pragma region オブジェクト3D
	object_ = std::make_unique<Object3D>(model_.get());
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
	PipelineManager::PreDraw("Object3D");

	// オブジェクト
	object_->Draw();
}

void GameOverScene::OnCollision()
{
}

void GameOverScene::MatUpdate()
{
	// カメラ
	camera_->Update();

	// オブジェクト3D
	object_->MatUpdate();
}
