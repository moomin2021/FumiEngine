#include "TestScene.h"
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
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 1.0f, -5.0f });
	Object3D::SetCamera(camera_.get());
	ParticleEmitter::SetCamera(camera_.get());
#pragma endregion

#pragma region ライト
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region パーティクル
	particle_ = std::make_unique<ParticleEmitter>();
	particleH_ = LoadTexture("Sprite/deathParticle.png");
#pragma endregion
}

void TestScene::Update()
{
	if (key_->PushKey(DIK_P))
	{
		particle_->Add(60, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 1.0f, 0.0f);
	}

	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void TestScene::Draw()
{
	PipelineManager::PreDraw("Particle");
	particle_->Draw(particleH_);
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
	particle_->Update(BILLBOARD::Y);
}
