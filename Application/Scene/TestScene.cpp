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
	for (size_t i = 0; i < cubeMax_; i++) colMgr_->RemoveCollider(&cubeC_[i]);
}

void TestScene::Initialize()
{
#pragma region インスタンス
	key_ = Key::GetInstance();
	lightGroup_ = LightGroup::GetInstance();
	Object3D::SetLightGroup(lightGroup_);
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 1.0f, -5.0f });
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ライト
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region モデル
	cubeM_ = std::make_unique<Model>("cube");
#pragma endregion

#pragma region オブジェクト
	cubeO_ = std::make_unique<Object3D[]>(cubeMax_);

	for (size_t i = 0; i < cubeMax_; i++)
	{
		cubeO_[i].SetModel(cubeM_.get());
		cubeO_[i].SetScale({ 0.5f, 0.5f, 0.5f });
	}
	cubeO_[0].SetPosition({ -0.6f, 0.0f, 0.0f });
	cubeO_[1].SetPosition({ 0.0f, 0.0f, -1.5f });
	cubeO_[2].SetPosition({ 0.6f, 0.0f, 0.0f });
#pragma endregion

#pragma region コライダー
	cubeC_ = std::make_unique<AABBCollider[]>(cubeMax_);

	for (size_t i = 0; i < cubeMax_; i++)
	{
		cubeC_[i].SetAttribute(1);
		cubeC_[i].SetRadius({ 0.5f, 0.5f, 0.5f });
		cubeC_[i].SetObject3D(&cubeO_[i]);
		colMgr_->AddCollider(&cubeC_[i]);
	}
#pragma endregion
}

void TestScene::Update()
{
	cube1Pos_.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.1f;
	cube1Pos_.y += (key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN)) * 0.1f;
	cube1Pos_.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.1f;

	cubeO_[1].SetPosition(cube1Pos_);

	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void TestScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	for (size_t i = 0; i < cubeMax_; i++) cubeO_[i].Draw();
}

void TestScene::Debug()
{

}

void TestScene::OnCollision()
{
	// 衝突判定をとる
	CollisionManager::GetInstance()->CheckAllCollision();

	for (size_t i = 0; i < cubeMax_; i++)
	{
		if (cubeC_[i].GetIsHit()) cubeO_[i].SetColor({ 0.0f, 1.0f, 1.0f, 1.0f });
		else cubeO_[i].SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	if (cubeC_[1].GetIsHit())
	{
		Vector3 reject = cubeC_[1].GetReject();
		cube1Pos_ += reject;
		cubeO_[1].SetPosition(cube1Pos_);
	}
}

void TestScene::MatUpdate()
{
	camera_->Update();
	for (size_t i = 0; i < cubeMax_; i++) cubeO_[i].MatUpdate();
}
