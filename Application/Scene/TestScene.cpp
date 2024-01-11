#include "TestScene.h"
#include "WinAPI.h"
#include "Texture.h"

#include "PipelineManager.h"
#include "CollisionAttribute.h"
#include "Util.h"

#include <set>
#include <imgui_impl_DX12.h>

TestScene::TestScene() {}

TestScene::~TestScene()
{
	lightGroup_->RemoveDirLight(dirLight_.get());
}

void TestScene::Initialize()
{
#pragma region カーソルの設定
	WinAPI::GetInstance()->DisplayCursor(false);
	WinAPI::GetInstance()->SetClipCursor(true);
#pragma endregion

#pragma region インスタンス
	key_ = Key::GetInstance();
	lightGroup_ = LightGroup::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -10.0f });
	ParticleEmitter::SetCamera(camera_.get());
#pragma endregion

#pragma region ライトグループ
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region パーティクルエミッター
	particleEmitter_ = std::make_unique<ParticleEmitter>();
	particleHandle_ = LoadTexture("Sprite/deathParticle.png");
#pragma endregion
}

void TestScene::Update()
{
	camera_->Update();

	pos_.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * speed_;
	pos_.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * speed_;
	particleEmitter_->SetSpawnPos(pos_);

	if (key_->PushKey(DIK_P))
	{
		CreateParticle();
		CreateParticle();
	}

	OnCollision();
	MatUpdate();
}

void TestScene::Draw()
{
	PipelineManager::PreDraw("Particle", D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	particleEmitter_->Draw(particleHandle_);
}

void TestScene::Debug()
{

}

void TestScene::OnCollision()
{

}

void TestScene::MatUpdate()
{
	particleEmitter_->Update();
}

void TestScene::CreateParticle()
{
	uint16_t life = 60;
	Vector3 pos = { 0.0f, 0.0f, 0.0f };
	Vector3 dir = { 1.0f, 0.0f, 0.0f };

	pos.x = Util::GetRandomFloat(-0.2f, 0.2f);
	pos.y = Util::GetRandomFloat(-0.2f, 0.2f);
	pos.z = Util::GetRandomFloat(-0.2f, 0.2f);

	particleEmitter_->Add(life, pos, dir * 0.1f, { 0.0f, 0.0f, 0.0f }, 1.0f, 0.0f);
}