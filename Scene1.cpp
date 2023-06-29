#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"

#include <DirectXMath.h>

using namespace DirectX;

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

	loadBle_ = std::make_unique<LoadBlender>();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// モデル
	mFloor_ = std::make_unique<Model>("floor");
	mCube_ = std::make_unique<Model>("cube");

	// オブジェクト
	oFloor_ = std::make_unique<Object3D>(mFloor_.get());
	oFloor_->SetScale({ 10.0f, 10.0f, 10.0f });

	oCube_.resize(3);

	oCube_[0] = std::make_unique<Object3D>(mCube_.get());
	oCube_[0]->SetPosition({ 3.0f, 1.0f, 0.0f });

	oCube_[1] = std::make_unique<Object3D>(mCube_.get());
	oCube_[1]->SetPosition({ -3.0f, 1.0f, 0.0f });

	oCube_[2] = std::make_unique<Object3D>(mCube_.get());
	oCube_[2]->SetPosition({ 0.0f, 1.0f, 0.0f });

	// テクスチャハンドル
	haeHandle_ = LoadTexture("Resources/hae.png");

	// スプライト
	sHae_ = std::make_unique<Sprite>();
	sHae_->SetPosition({ 0.0f, 0.0f });

	// ライト生成
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();
	pointLight_ = std::make_unique<PointLight>();

	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	pointLight_->SetLightPos({ -4.0f, 1.0f, 0.0f });

	lightGroup_->AddDirLight(dirLight_.get());
	//lightGroup_->AddPointLight(pointLight_.get());

	// カメラを設定
	Object3D::SetCamera(camera_.get());

	// ライトを設定
	Object3D::SetLightGroup(lightGroup_.get());
}

void Scene1::Update()
{
	// カメラ移動
	{
		static float3 eye = { 0.0f, 10.0f, -30.0f };

		eye.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		eye.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;

		camera_->SetEye(eye);
	}

	static float3 pointLightPos = { -4.0f, 1.0f, 0.0f };

	pointLightPos.x += (key_->PushKey(DIK_RIGHT) - key_->PushKey(DIK_LEFT)) * 0.2f;
	pointLightPos.z += (key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN)) * 0.2f;

	pointLight_->SetLightPos(pointLightPos);

	static float rota = 0.0f;
	rota += 1.0f;

	oCube_[0]->SetRotation({ 0.0f, rota, 0.0f });
	oCube_[1]->SetRotation({0.0f, rota, 0.0f});

	// カメラの更新
	camera_->Update();
}

void Scene1::Draw()
{
	PipelineManager::GetInstance()->PreDraw("Object3D");

	loadBle_->Draw();
	//oFloor_->Draw();
	//for (auto& object : oCube_) object->Draw();

	PipelineManager::GetInstance()->PreDraw("Sprite");

	//sHae_->Draw();
}