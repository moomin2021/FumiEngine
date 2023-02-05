#include "Scene1.h"

Scene1::Scene1() :
	key_(nullptr),
	camera_(nullptr),
	lightGroup_(nullptr),
	sphereM_(nullptr),
	object_{}
{
}

Scene1::~Scene1()
{
	delete camera_;
	delete lightGroup_;
	delete sphereM_;
	delete cubeM_;
	delete floorM_;
	delete object_[0];
	delete object_[1];
	delete object_[2];
}

void Scene1::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// モデル
	//sphereM_ = Model::CreateModel("sphere");
	cubeM_ = Model::CreateModel("cube");
	floorM_ = Model::CreateModel("floor");

	// オブジェクト
	object_[0] = Object3D::CreateObject3D(cubeM_);
	object_[0]->SetPos({ 3.0f, 1.0f, 0.0f });

	object_[1] = Object3D::CreateObject3D(cubeM_);
	object_[1]->SetPos({ -3.0f, 1.0f, 0.0f });

	object_[2] = Object3D::CreateObject3D(floorM_);
	object_[2]->SetPos({ 0.0f, 0.0f, 0.0f });
	object_[2]->SetScale({ 10.0f, 10.0f, 10.0f });

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

	if (key_->TriggerKey(DIK_E)) {
		object_[0]->SetModel(cubeM_);
	}

	if (key_->TriggerKey(DIK_Q)) {
		object_[0]->SetModel(sphereM_);
	}

	static Float3 pos = { 0.0f, 0.5f, 0.0f };

	pos.x += (key_->PushKey(DIK_RIGHT) - key_->PushKey(DIK_LEFT)) * 0.2f;
	pos.z += (key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN)) * 0.2f;

	lightGroup_->SetPointLightPos(0, XMFLOAT3({ pos.x, pos.y, pos.z }));

	static float rota = 0.0f;
	rota += 1.0f;

	object_[0]->SetRot({ 0.0f, rota, 0.0f });
	object_[1]->SetRot({0.0f, rota, 0.0f});

	// カメラの更新
	camera_->Update();

	// ライトの更新
	lightGroup_->Update();
}

void Scene1::Draw()
{
	Object3D::PreDraw();

	for (auto& object : object_) object->Draw();
}