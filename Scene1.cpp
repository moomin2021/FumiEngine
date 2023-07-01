#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"

#include <DirectXMath.h>

using namespace DirectX;

Scene1::Scene1() :
	// カメラ
	camera_(nullptr),

	lightGroup_(nullptr),
	dirLight_(nullptr)
{
}

Scene1::~Scene1()
{
	
}

void Scene1::Initialize()
{
	// カメラを生成＆適用
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// カメラを適用
	Object3D::SetCamera(camera_.get());

	// ライト生成＆設定
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());

	// ライトを適用
	Object3D::SetLightGroup(lightGroup_.get());
}

void Scene1::Update()
{
	// カメラの更新
	camera_->Update();
}

void Scene1::Draw()
{

}