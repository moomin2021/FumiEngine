#include "Scene1.h"

Scene1::Scene1() :
	key_(nullptr),
	camera_(nullptr),
	sphereM_(nullptr),
	sphereObj_(nullptr)
{
}

Scene1::~Scene1()
{
	delete camera_;
	delete sphereM_;
	delete sphereObj_;
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
	sphereObj_ = Object3D::CreateObject3D();
	sphereObj_->SetCamera(camera_);
	sphereObj_->SetModel(sphereM_);
}

void Scene1::Update()
{
	// カメラの更新
	camera_->Update();

	static float rota = 0.0f;
	rota += 1.0f;

	sphereObj_->SetRot({ 0.0f, rota, 0.0f });
}

void Scene1::Draw()
{
	Object3D::PreDraw();

	sphereObj_->Draw();
}