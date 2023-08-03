#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "Sound.h"
#include "CollisionManager.h"

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
	camera_->SetEye({ 0.0f, 1.0f, -20.0f });

	// カメラセット
	Object3D::SetCamera(camera_.get());
	ParticleEmitter::SetCamera(camera_.get());

	// ライトグループ生成
	lightGroup_ = std::make_unique<LightGroup>();

	// ライトグループセット
	Object3D::SetLightGroup(lightGroup_.get());

	// 平行光源生成
	dirLight_ = std::make_unique<DirectionalLight>();

	// 平行光源セット
	lightGroup_->AddDirLight(dirLight_.get());

	// モデル生成
	model_ = std::make_unique<Model>("sphere");

	// オブジェクト生成
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition({ -1.0f, 0.0f, 0.0f });

	particleEmitters_.resize(10);
	// パーティクルエミッター生成
	for (size_t i = 0; i < 10; i++) {
		particleEmitters_[i] = std::make_unique<ParticleEmitter>();
		particleEmitters_[i]->SetSpawnPos({(i * 1.0f) - 4.5f, 0.0f, 0.0f});

		for (size_t j = 0; j < 1000; j++) {
			// パーティクル生成
			float3 pos{};// 座標
			pos.x = Util::GetRandomFloat(-0.1f, 0.1f);
			pos.y = Util::GetRandomFloat(-0.1f, 0.1f);
			pos.z = Util::GetRandomFloat(-0.1f, 0.1f);

			// 方向
			float3 vel{};
			vel.x = Util::GetRandomFloat(-0.1f, 0.1f);
			vel.y = Util::GetRandomFloat(-0.1f, 0.1f);
			vel.z = Util::GetRandomFloat(-0.1f, 0.1f);

			// 加速度
			float3 acc{};
			acc.x = Util::GetRandomFloat(-0.001f, 0.0f);
			acc.y = Util::GetRandomFloat(-0.001f, 0.0f);
			acc.z = Util::GetRandomFloat(-0.001f, 0.0f);

			particleEmitters_[i]->Add(1000, pos, vel, acc, 0.5f, 0.0f);
		}
	}

	// パーティクル用画像読み込み
	particlehandle_ = LoadTexture("Resources/effect1.png");

	// サウンド読み込み＆再生
	bgmKey_ = Sound::LoadWave("Resources/Sound/a.wav");
	Sound::SetVolume(bgmKey_, 0.001f);
	Sound::Play(bgmKey_);
}

void Scene1::Update()
{
	// パーティクル更新
	for (auto& i : particleEmitters_) {
		i->Update(BILLBOARD::ALL);
	}
 
	// オブジェクト更新
	object_->Update();

	// カメラの更新
	camera_->Update();

	// 衝突判定
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// オブジェクト描画
	object_->Draw();

	PipelineManager::PreDraw("Particle", D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	// パーティクル描画
	for (auto& i : particleEmitters_) {
		i->Draw(particlehandle_);
	}
}