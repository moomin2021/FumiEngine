#include "MapCreateScene.h"
#include "PipelineManager.h"

MapCreateScene::MapCreateScene(IScene* sceneIf) : BaseScene(sceneIf) {}

void MapCreateScene::Initialize()
{
#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -10.0f });
	camera_->SetTarget({ 0.0f, 0.0f, 10.0f });
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ライトグループ
	lightGroup_ = LightGroup::GetInstance();
	Object3D::SetLightGroup(lightGroup_);

	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region スカイドーム
	// モデル
	mSkydome_ = std::make_unique<Model>("skydome");

	// オブジェクト
	oSkydome_ = std::make_unique<Object3D>(mSkydome_.get());
	oSkydome_->SetScale({ 1000.0f, 1000.0f, 1000.0f });
#pragma endregion

	model_ = std::make_unique<Model>("3Way_0");

	object_ = std::make_unique<Object3D>(model_.get());
}

void MapCreateScene::Update()
{
	Collision();
	MatUpdate();
	Debug();
}

void MapCreateScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	oSkydome_->Draw();
	object_->Draw();
}

void MapCreateScene::Finalize()
{
}

void MapCreateScene::Collision()
{

}

void MapCreateScene::MatUpdate()
{
	camera_->Update();

	oSkydome_->MatUpdate();
	object_->MatUpdate();
}

void MapCreateScene::Debug()
{
}