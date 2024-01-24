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
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ライトグループ
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

	model_ = std::make_unique<Model>("zombie");

	object_ = std::make_unique<Object3D>(model_.get());

#pragma region パーティクルエミッター
	particleEmitter_ = std::make_unique<ParticleEmitter>();
	particleHandle_ = LoadTexture("Sprite/dot.png");

	smokeP_ = std::make_unique<ParticleEmitter>();
	smokeHandle_ = LoadTexture("Sprite/smoke.png");
#pragma endregion
}

void TestScene::Update()
{
	camera_->Update();

	pos_.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * speed_;
	pos_.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * speed_;
	particleEmitter_->SetSpawnPos(pos_);
	smokeP_->SetSpawnPos(pos_);

	if (key_->PushKey(DIK_P))
	{
		for (uint16_t i = 0; i < 20; i++)
		{
			CreateParticle(i);
		}
		//CreateSmoke();
	}

	if (key_->TriggerKey(DIK_O))
	{
		for (uint16_t i = 0; i < 50; i++)
		{
			CreateParticle(i);
		}
		//CreateSmoke();
	}

	OnCollision();
	MatUpdate();
}

void TestScene::Draw()
{
	PipelineManager::PreDraw("Object3D");
	object_->Draw();

	PipelineManager::PreDraw("Particle", D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	particleEmitter_->Draw(particleHandle_);
	smokeP_->Draw(smokeHandle_);
}

void TestScene::Debug()
{

}

void TestScene::OnCollision()
{

}

void TestScene::MatUpdate()
{
	object_->MatUpdate();
	particleEmitter_->Update();
	smokeP_->Update();
}

void TestScene::CreateParticle(uint16_t i)
{
	uint16_t life = 10;
	Vector3 pos = { 0.0f, 0.0f, 0.0f };
	Vector3 dir = { 1.0f, 0.0f, 0.0f };

	float a = 0.2f * (i / 50.0f);

	dir.x = Util::GetRandomFloat(-0.1f, 0.1f);
	dir.y = Util::GetRandomFloat(0.1f, 0.1f + a);
	dir.z = Util::GetRandomFloat(-0.1f, 0.1f);

	particleEmitter_->Add(life, pos, dir, -dir / 10.0f, 1.0f / 16.0f, 1.0f / 16.0f);
}

void TestScene::CreateSmoke()
{
	uint16_t life = 10;
	Vector3 dir = { 0.0f, 0.15f, 0.0f };

	smokeP_->Add(life, Vector3(), Vector3(), Vector3(), 1.0f, 0.0f);
}