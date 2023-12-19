#include "TestScene.h"
#include "CollisionManager.h"
#include "WinAPI.h"
#include "Texture.h"

#include "PipelineManager.h"

#include <set>

TestScene::TestScene() {}

TestScene::~TestScene()
{
	lightGroup_->RemoveDirLight(dirLight_.get());
}

void TestScene::Initialize()
{
#pragma region インスタンス
	key_ = Key::GetInstance();
	lightGroup_ = LightGroup::GetInstance();
	Object3D::SetLightGroup(lightGroup_);
	Instancing3D::SetLightGroup(lightGroup_->GetInstance());
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 1.0f, -5.0f });
	Object3D::SetCamera(camera_.get());
	Instancing3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ライト
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region モデル
	model_ = std::make_unique<Model>("stoneBrick");
#pragma endregion

	objects_ = std::make_unique<Instancing3D>((uint16_t)1, model_.get());

	for (size_t i = 0; i < 1; i++)
	{
		objects_->AddTransform({ -4.5f + (i * 1.0f), 0.0f, 0.0f });
	}
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
	PipelineManager::PreDraw("Instancing3D");

	objects_->Draw();
}

void TestScene::Debug()
{

}

void TestScene::OnCollision()
{

}

void TestScene::MatUpdate()
{
	camera_->Update();
	objects_->MatUpdate();
}
