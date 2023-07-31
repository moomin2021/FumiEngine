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

	// モデル
	mFloor_ = std::make_unique<Model>("floor");
	mCube_ = std::make_unique<Model>("cube");
	mSphere_ = std::make_unique<Model>("sphere");

	// オブジェクト
	oFloor_ = std::make_unique<Object3D>(mFloor_.get());
	oFloor_->SetScale({ 10.0f, 10.0f, 10.0f });

	oSphere_ = std::make_unique<Object3D>(mSphere_.get());
	oSphere_->SetPosition({ 3.0f, 1.0f, 0.0f });
	oSphere_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	oCube_.resize(3);

	oCube_[0] = std::make_unique<Object3D>(mCube_.get());
	oCube_[0]->SetPosition({ 0.0f, 1.0f, 3.0f });

	oCube_[1] = std::make_unique<Object3D>(mCube_.get());
	oCube_[1]->SetPosition({ -3.0f, 1.0f, 0.0f });

	oCube_[2] = std::make_unique<Object3D>(mCube_.get());
	oCube_[2]->SetPosition({ 0.0f, 1.0f, 0.0f });

	sphereCollider_ = std::make_unique<SphereCollider>();
	sphereCollider_->LinkObject3D(oCube_[1].get());
	sphereCollider_->AddAttribute(COL_ATTR_PLAYER);

	meshCollider_ = std::make_unique<MeshCollider>(oCube_[2].get());

	rayCollider_ = std::make_unique<RayCollider>();
	rayCollider_->LinkObject3D(oSphere_.get());
	rayCollider_->SetDir({ -1.0f, 0.0f, 0.0f });
	rayCollider_->AddAttribute(COL_ATTR_PLAYER);

	CollisionManager::GetInstance()->AddCollider(sphereCollider_.get());
	CollisionManager::GetInstance()->AddCollider(meshCollider_.get());
	CollisionManager::GetInstance()->AddCollider(rayCollider_.get());

	particle_ = std::make_unique<ParticleManager>();
	for (size_t i = 0; i < 100; i++) {
		float3 pos{};
		pos.x = Util::GetRandomFloat(-5.0f, 5.0f);
		pos.y = Util::GetRandomFloat(-5.0f, 5.0f);
		pos.z = Util::GetRandomFloat(-5.0f, 5.0f);

		float3 vel{};
		vel.x = Util::GetRandomFloat(-0.05f, 0.05f);
		vel.y = Util::GetRandomFloat(-0.05f, 0.05f);
		vel.z = Util::GetRandomFloat(-0.05f, 0.05f);

		float3 acc{};
		acc.y = -Util::GetRandomFloat(-0.001f, 0.0f);

		particle_->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}

	// テクスチャハンドル
	haeHandle_ = LoadTexture("Resources/effect1.png");

	// スプライト
	sHae_ = std::make_unique<Sprite>();
	sHae_->SetPosition({ 0.0f, 0.0f });

	// ライト生成
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();
	pointLight_ = std::make_unique<PointLight>();

	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	//pointLight_->SetLightPos({ -4.0f, 1.0f, 0.0f });

	lightGroup_->AddDirLight(dirLight_.get());
	//lightGroup_->AddPointLight(pointLight_.get());

	// カメラを設定
	Object3D::SetCamera(camera_.get());

	// ライトを設定
	Object3D::SetLightGroup(lightGroup_.get());

	// パーティクルにカメラを設定
	ParticleManager::SetCamera(camera_.get());

	// 音声
	sound_ = Sound::SoundLoadWave("Resources/Sound/a.wav");
}

void Scene1::Update()
{
	// カメラ移動
	{
		static float3 eye = { 0.0f, 1.0f, -5.0f };

		eye.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		eye.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;

		camera_->SetEye(eye);
	}

	static float3 spherePos = { 3.0f, 1.0f, -3.0f };

	spherePos.x += (key_->PushKey(DIK_RIGHT) - key_->PushKey(DIK_LEFT)) * 0.2f;
	spherePos.z += (key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN)) * 0.2f;

	oSphere_->SetPosition(spherePos);

	static float rota = 0.0f;
	rota += 1.0f;

	oCube_[0]->SetRotation({ 0.0f, rota, 0.0f });
	oCube_[1]->SetRotation({0.0f, rota, 0.0f});

	oSphere_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	oCube_[1]->SetColor({ 1.0f, 1.0f,1.0f, 1.0f });
	oCube_[2]->SetColor({ 1.0f, 1.0f,1.0f, 1.0f });

	int num = 0;
	if (Key::GetInstance()->TriggerKey(DIK_0)) {
		num = 0;
	}

	if (sphereCollider_->GetIsHit()) oCube_[1]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	if (meshCollider_->GetIsHit()) oCube_[2]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	if (rayCollider_->GetIsHit()) oSphere_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	// オブジェクトの更新
	oFloor_->Update();
	oSphere_->Update();
	for (auto& i : oCube_) i->Update();

	if (key_->TriggerKey(DIK_L)) {
		Sound::SoundPlay(sound_);
	}

	// カメラの更新
	camera_->Update();

	// パーティクル更新
	particle_->Update(BILLBOARD::ALL);

	// 衝突判定
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene1::Draw()
{
	PipelineManager::GetInstance()->PreDraw("Object3D");

	//oFloor_->Draw();
	//oSphere_->Draw();
	//for (auto& object : oCube_) object->Draw();

	PipelineManager::GetInstance()->PreDraw("Particle", D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	particle_->Draw(haeHandle_);

	//PipelineManager::GetInstance()->PreDraw("Sprite");

	//sHae_->Draw();
}