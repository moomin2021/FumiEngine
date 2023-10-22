#include "TestScene.h"

#include "PipelineManager.h"

TestScene::TestScene() {}

TestScene::~TestScene() {}

void TestScene::Initialize()
{
	//element2.svPos = input[(i + 1) % 2].svPos + float4(offset * sign(i - 1), 0.0, 0.0, 0.0);

	Vector3 result = Vector3(2.0f, 0.0f, 0.0f) + Vector3(2.5f * sinf(0 - 1), 0.0f, 0.0f);

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 0.0f, -10.0f });
	Object3D::SetCamera(camera_.get());
	Line3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ライトグループ
	lightGroup_ = std::make_unique<LightGroup>();
	Object3D::SetLightGroup(lightGroup_.get());

	dirLight_ = std::make_unique<DirectionalLight>();
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region モデル
	model_ = std::make_unique<Model>("cube");
#pragma endregion

#pragma region オブジェクト3D
	object_ = std::make_unique<Object3D>(model_.get());
#pragma endregion

#pragma region 線
	line_ = std::make_unique<Line3D>();
	line_->Initialize(100);
	line_->AddPoint(Vector3(-2.0f, 0.0f, 0.0f), Vector3(0.0f, 2.0f, 0.0f));
	line_->AddPoint(Vector3(2.0f, 0.0f, 0.0f), Vector3(2.0f, 2.0f, 0.0f));
#pragma endregion
}

void TestScene::Update()
{
	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void TestScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// オブジェクト
	//object_->Draw();

	//PipelineManager::PreDraw("Line3D");
	PipelineManager::PreDraw("Line3D", D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	// 線
	line_->Draw();
}

void TestScene::OnCollision()
{
}

void TestScene::MatUpdate()
{
	// カメラ
	camera_->Update();

	// オブジェクト3D
	object_->MatUpdate();

	// 線
	line_->MatUpdate();
}
