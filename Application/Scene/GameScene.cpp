#include "GameScene.h"

#include "PipelineManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize()
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

#pragma region ステージオブジェクトの管理クラス
	stageObjMgr_ = std::make_unique<StageObjectManager>();
#pragma endregion
}

void GameScene::Update()
{
	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void GameScene::Draw()
{
	// ステージオブジェクトの管理クラス
	stageObjMgr_->Draw();
}

void GameScene::OnCollision()
{
}

void GameScene::MatUpdate()
{
	// カメラ
	camera_->Update();

	// ステージオブジェクトの管理クラス
	stageObjMgr_->MatUpdate();
}