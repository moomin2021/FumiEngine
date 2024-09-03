#include "GameScene.h"

GameScene::GameScene(IScene* sceneIf) : BaseScene(sceneIf) {}

void GameScene::Initialize()
{
#pragma region デバックカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
#pragma endregion

#pragma region カメラマネージャー
	cameraMgr_ = std::make_unique<CameraManager>();
	cameraMgr_->Initialize(debugCamera_->GetCamera());
#pragma endregion
}

void GameScene::Update()
{
	// 衝突時処理
	Collision();

	// 行列更新処理
	MatUpdate();

	// デバック
	Debug();
}

void GameScene::Draw()
{

}

void GameScene::Finalize()
{

}

void GameScene::Debug()
{

}

void GameScene::Collision()
{
	
}

void GameScene::MatUpdate()
{
	cameraMgr_->MatUpdate();
	debugCamera_->MatUpdate();
}
