#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

Scene1::Scene1() :
	key_(nullptr)
{
}

Scene1::~Scene1()
{
	
}

void Scene1::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 1.0f, -5.0f });

	particle_ = std::make_unique<ParticleManager>();
	for (size_t i = 0; i < 100; i++) {
		float3 pos{};
		pos.x = Util::GetRandomFloat(-5.0f, 5.0f);
		pos.y = Util::GetRandomFloat(-5.0f, 5.0f);
		pos.z = Util::GetRandomFloat(-5.0f, 5.0f);

	particles_[0]->SetSpawnPos({ -2.0f, 1.0f, 0.0f });
	particles_[1]->SetSpawnPos({ 0.0f, 1.0f, 0.0f });
	particles_[2]->SetSpawnPos({ 2.0f, 1.0f, 0.0f });

		float3 acc{};
		acc.y = -Util::GetRandomFloat(-0.001f, 0.0f);

		particle_->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}

	// パーティクルにカメラを設定
	ParticleEmitter::SetCamera(camera_.get());

	// パーティクルの画像読み込み
	particleHandle_ = LoadTexture("Resources/effect1.png");

	// 音声
	soundKey_ = Sound::LoadWave("Resources/Sound/TestBGM.wav");
	Sound::SetVolume(soundKey_, 0.001f);
}

void Scene1::Update()
{
	// カメラ移動
	{
		static float3 eye = { 0.0f, 3.0f, -20.0f };

		eye.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		eye.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;

		camera_->SetEye(eye);
	}
 
	// カメラの更新
	camera_->Update();

	// パーティクル更新
	particles_[0]->Update(BILLBOARD::ALL);
	particles_[1]->Update(BILLBOARD::ALL);
	particles_[2]->Update(BILLBOARD::ALL);

	// 衝突判定
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene1::Draw()
{
	PipelineManager::GetInstance()->PreDraw("Particle", D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// パーティクル描画
	particle_->Draw(particleHandle_);
}