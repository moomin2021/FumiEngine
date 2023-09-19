#include "TestScene.h"

TestScene::TestScene() {}

TestScene::~TestScene() {}

void TestScene::Initialize()
{
#pragma region カメラ
	camera_ = std::make_unique<Camera>();
#pragma endregion
}

void TestScene::Update()
{
	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void TestScene::Draw()
{
}

void TestScene::OnCollision()
{
}

void TestScene::MatUpdate()
{
	// カメラ
	camera_->Update();
}