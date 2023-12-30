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
	camera_->SetEye({ 0.0f, 1.0f, -10.0f });
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

	Vector3 pos;
	pos.x = Util::GetRandomFloat(-5.0f, 5.0f);
	pos.y = Util::GetRandomFloat(-5.0f, 5.0f);
	pos.z = Util::GetRandomFloat(-5.0f, 5.0f);

	for (size_t i = 0; i < 10; i++)
	{
		Vector3 offset;
		offset.x = Util::GetRandomFloat(-0.5f, 0.5f);
		offset.y = Util::GetRandomFloat(-0.5f, 0.5f);
		offset.z = Util::GetRandomFloat(-0.5f, 0.5f);

		Vector3 velocity = { 0.0f, 1.0f, 0.0f };
		velocity.x = Util::GetRandomFloat(-0.5f, 0.5f);
		velocity.z = Util::GetRandomFloat(-0.5f, 0.5f);
		velocity.normalize();
		velocity /= 60.0f;
		particle_->Add(60, pos + offset, velocity, { -velocity.x / 60.0f, -velocity.y / 60.0f, -velocity.z / 60.0f }, 1.0f, 0.0f);
	}
#pragma endregion
}

void TestScene::Update()
{
	if (key_->TriggerKey(DIK_P))
	{
		Vector3 pos;
		pos.x = Util::GetRandomFloat(-5.0f, 5.0f);
		pos.y = Util::GetRandomFloat(-5.0f, 5.0f);
		pos.z = Util::GetRandomFloat(-5.0f, 5.0f);

		for (size_t i = 0; i < 10; i++)
		{
			Vector3 offset;
			offset.x = Util::GetRandomFloat(-0.5f, 0.5f);
			offset.y = Util::GetRandomFloat(-0.5f, 0.5f);
			offset.z = Util::GetRandomFloat(-0.5f, 0.5f);

			Vector3 velocity = { 0.0f, 1.0f, 0.0f };
			velocity.x = Util::GetRandomFloat(-0.5f, 0.5f);
			velocity.z = Util::GetRandomFloat(-0.5f, 0.5f);
			velocity.normalize();
			velocity /= 60.0f;
			particle_->Add(60, pos + offset, velocity, { -velocity.x / 60.0f, -velocity.y / 60.0f, -velocity.z / 60.0f }, 1.0f, 0.0f);
		}
	}

	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void TestScene::Draw()
{
	PipelineManager::PreDraw("Particle", D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
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
