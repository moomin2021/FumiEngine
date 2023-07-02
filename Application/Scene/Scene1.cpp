#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"

#include <DirectXMath.h>

using namespace DirectX;

Scene1::Scene1() :
	lightGroup_(nullptr),
	dirLight_(nullptr),

	// ステージ読み込み用
	loadStage_(nullptr),

	// プレイヤー
	player_(nullptr)
{
}

Scene1::~Scene1()
{
	
}

void Scene1::Initialize()
{
	// ライト生成＆設定
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());

	// ライトを適用
	Object3D::SetLightGroup(lightGroup_.get());

	// ステージ読み込み用クラス生成
	loadStage_ = std::make_unique<LoadStage>();

	// プレイヤー生成
	player_ = std::make_unique<Player>();
}

void Scene1::Update()
{
	// ステージ更新
	loadStage_->Update();

	// プレイヤー更新
	player_->Update();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// ステージ描画
	loadStage_->Draw();
}