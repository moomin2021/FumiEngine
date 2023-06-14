#include "Scene1.h"
#include "Texture.h"

Scene1::Scene1() :
	key_(nullptr),
	camera_(nullptr),
	lightGroup_(nullptr)
{
}

Scene1::~Scene1()
{
	delete camera_;
	delete lightGroup_;
	delete mCube_;
	for (auto& object : oCube_) delete object;
	delete sHae_;
}

void Scene1::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// モデル
	mCube_ = Model::CreateModel("cube");

	// オブジェクト
	oCube_.resize(3);

	oCube_[0] = Object3D::CreateObject3D(mCube_);
	oCube_[0]->SetPos({ 3.0f, 1.0f, 0.0f });

	oCube_[1] = Object3D::CreateObject3D(mCube_);
	oCube_[1]->SetPos({ -3.0f, 1.0f, 0.0f });

	oCube_[2] = Object3D::CreateObject3D(mCube_);
	oCube_[2]->SetPos({ 0.0f, 0.0f, 0.0f });

	// テクスチャハンドル
	haeHandle_ = LoadTexture("Resources/hae.png");

	// スプライト
	sHae_ = new Sprite();

	// ライト生成
	lightGroup_ = LightGroup::Create();

	lightGroup_->SetDirLightActive(0, true);
	lightGroup_->SetDirLightActive(1, false);
	lightGroup_->SetDirLightActive(2, false);
	lightGroup_->SetPointLightActive(0, true);
	lightGroup_->SetPointLightActive(1, false);
	lightGroup_->SetPointLightActive(2, false);
	lightGroup_->SetSpotLightActive(0, false);
	lightGroup_->SetSpotLightActive(1, false);
	lightGroup_->SetSpotLightActive(2, false);
	lightGroup_->SetCircleShadowActive(0, false);

	// カメラを設定
	Object3D::SetCamera(camera_);

	// ライトを設定
	Object3D::SetLightGroup(lightGroup_);
}

void Scene1::Update()
{
	if (key_->TriggerKey(DIK_0)) {
		int num = 0;
	}

	// カメラ移動
	{
		camera_->eye_.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		camera_->eye_.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;
	}

	static float3 pos = { 0.0f, 0.5f, 0.0f };

	pos.x += (key_->PushKey(DIK_RIGHT) - key_->PushKey(DIK_LEFT)) * 0.2f;
	pos.z += (key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN)) * 0.2f;

	lightGroup_->SetPointLightPos(0, XMFLOAT3({ pos.x, pos.y, pos.z }));

	static float rota = 0.0f;
	rota += 1.0f;

	oCube_[0]->SetRot({ 0.0f, rota, 0.0f });
	oCube_[1]->SetRot({0.0f, rota, 0.0f});

	sHae_->Update();

	// カメラの更新
	camera_->Update();

	// ライトの更新
	lightGroup_->Update();
}

void Scene1::Draw()
{
	Object3D::PreDraw();

	for (auto& object : oCube_) object->Draw();

	Sprite::PreDraw();

	sHae_->Draw(haeHandle_);
}