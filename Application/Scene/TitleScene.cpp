#include "TitleScene.h"

#include "PipelineManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize()
{
#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 0.0f, -10.0f });
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ライトグループ
	lightGroup_ = std::make_unique<LightGroup>();
	Object3D::SetLightGroup(lightGroup_.get());

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

void TitleScene::Update()
{
	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// オブジェクト
	object_->Draw();
}

void TitleScene::OnCollision()
{
}

void TitleScene::MatUpdate()
{
	// カメラ
	camera_->Update();

	// オブジェクト3D
	object_->MatUpdate();
}