#include "TestScene.h"
#include "WinAPI.h"
#include "Texture.h"

#include "PipelineManager.h"
#include "CollisionAttribute.h"
#include "Util.h"

#include <set>
#include <imgui_impl_DX12.h>

TestScene::TestScene() {}

TestScene::~TestScene()
{
	lightGroup_->RemoveDirLight(dirLight_.get());
}

void TestScene::Initialize()
{
#pragma region カーソルの設定
	// オフ
	WinAPI::GetInstance()->DisplayCursor(false);
	WinAPI::GetInstance()->SetClipCursor(true);

	// オン
	WinAPI::GetInstance()->DisplayCursor(true);
	WinAPI::GetInstance()->SetClipCursor(false);
#pragma endregion

#pragma region インスタンス
	key_ = Key::GetInstance();
	mouse_ = Mouse::GetInstance();
	lightGroup_ = LightGroup::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -0.5f });
	ParticleEmitter::SetCamera(camera_.get());
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ライトグループ
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

	model_ = std::make_unique<Model>("stoneBrick");

	object_ = std::make_unique<Object3D>(model_.get());
}

void TestScene::Update()
{
	camera_->Update();

	Vector3 mousePos = { mouse_->MousePos().x, mouse_->MousePos().y, 0.0f };

	Matrix4 invView, invPrj, vp, invViewport;
	invView = Matrix4Inverse(camera_->GetMatView());
	invPrj = Matrix4Inverse(camera_->GetMatProjection());
	vp = Matrix4Identity();
	vp.m[0][0] = 1920.0f / 2.0f, vp.m[1][1] = -1080.0f / 2.0f;
	vp.m[3][0] = 1920.0f / 2.0f, vp.m[3][1] = 1080.0f / 2.0f;
	invViewport = Matrix4Inverse(vp);

	Matrix4 tmp = invViewport * invPrj * invView;
	result0_ = Matrix4Transform(mousePos, tmp);
	mousePos.z = 1.0f;
	result1_ = Matrix4Transform(mousePos, tmp);

	OnCollision();
	MatUpdate();
	Debug();
}

void TestScene::Draw()
{
	PipelineManager::PreDraw("Object3D");
	object_->Draw();
}

void TestScene::Debug()
{
	ImGui::Begin("Debug");
	ImGui::Text("result = {%f, %f, %f}", result0_.x, result0_.y, result0_.z);
	ImGui::Text("result = {%f, %f, %f}", result1_.x, result1_.y, result1_.z);

	Vector3 ray = result1_ - result0_;
	ray.normalize();

	ImGui::Text("ray = {%f, %f, %f}", ray.x, ray.y, ray.z);

	ImGui::End();
}

void TestScene::OnCollision()
{

}

void TestScene::MatUpdate()
{
	object_->MatUpdate();
}