#include "TestScene.h"
#include "CollisionManager.h"
#include "WinAPI.h"
#include "Texture.h"

#include "PipelineManager.h"

#include <set>

TestScene::TestScene() {}

TestScene::~TestScene() {}

void TestScene::Initialize()
{
#pragma region インスタンス
	key_ = Key::GetInstance();
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

void TestScene::Debug()
{

}

void TestScene::OnCollision()
{
	// 衝突判定をとる
	CollisionManager::GetInstance()->CheckAllCollision();
}

void TestScene::MatUpdate()
{

}
