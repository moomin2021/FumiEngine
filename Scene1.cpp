#include "Scene1.h"

Scene1::Scene1() :
	key_(nullptr),
	camera_(nullptr),
	sphereM_(nullptr),
	sphereObj_{}
{
}

Scene1::~Scene1()
{
	delete camera_;
	delete sphereM_;
	delete sphereObj_[0];
	delete sphereObj_[1];
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

	// オブジェクト
	sphereObj_[0] = Object3D::CreateObject3D();
	sphereObj_[0]->SetPos({ 3.0f, 0.0f, 0.0f });
	sphereObj_[0]->SetModel(sphereM_);

	sphereObj_[1] = Object3D::CreateObject3D();
	sphereObj_[1]->SetPos({ -3.0f, 0.0f, 0.0f });
	sphereObj_[1]->SetModel(sphereM_);

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

	static float rota = 0.0f;
	rota += 1.0f;

	sphereObj_[0]->SetRot({ 0.0f, rota, 0.0f });
	sphereObj_[1]->SetRot({0.0f, rota, 0.0f});

	// カメラの更新
	camera_->Update();
}

void Scene1::Draw()
{
	Object3D::PreDraw();

	sphereObj_[0]->Draw();
	sphereObj_[1]->Draw();
}