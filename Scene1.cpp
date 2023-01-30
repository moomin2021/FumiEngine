#include "Scene1.h"

Scene1::Scene1() :
	key_(nullptr),
	camera_(nullptr),
	sphereM_(nullptr),
	object_{}
{
}

Scene1::~Scene1()
{
	delete camera_;
	delete sphereM_;
	delete cubeM_;
	delete object_[0];
	delete object_[1];
}

void Scene1::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// モデル
	sphereM_ = Model::CreateModel("sphere");
	cubeM_ = Model::CreateModel("cube");

	// オブジェクト
	object_[0] = Object3D::CreateObject3D(sphereM_);
	object_[0]->SetPos({ 3.0f, 0.0f, 0.0f });

	object_[1] = Object3D::CreateObject3D(sphereM_);
	object_[1]->SetPos({ -3.0f, 0.0f, 0.0f });

	// カメラを設定
	Object3D::SetCamera(camera_);
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

	static float rota = 0.0f;
	rota += 1.0f;

	object_[0]->SetRot({ 0.0f, rota, 0.0f });
	object_[1]->SetRot({0.0f, rota, 0.0f});

	// カメラの更新
	camera_->Update();
}

void Scene1::Draw()
{
	Object3D::PreDraw();

	object_[0]->Draw();
	object_[1]->Draw();
}