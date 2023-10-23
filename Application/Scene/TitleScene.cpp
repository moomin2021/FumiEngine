#include "TitleScene.h"
#include "Texture.h"
#include "PipelineManager.h"
#include "SceneManager.h"
#include "CollisionAttribute.h"
#include "Util.h"
#include "Easing.h"

#include <imgui_impl_DX12.h>

TitleScene::TitleScene() {}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
#pragma region インスタンス取得
	colMgr2D_ = CollisionManager2D::GetInstance();// 衝突マネージャー2D
#pragma endregion

#pragma region ライトグループ
	lightGroup_ = std::make_unique<LightGroup>();
	Object3D::SetLightGroup(lightGroup_.get());

	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 20.0f, -90.0f });
	camera_->SetTarget({ 0.0f, 10.0f, 0.0f });
	Sprite::SetCamera(camera_.get());
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ステージ
	stage_ = std::make_unique<Stage>();
	stage_->Initialize();
	stage_->Load("Resources/StageJson/stage1.json", false);
#pragma endregion

#pragma region タイトルレイヤー
	titleLayer_ = std::make_unique<TitleLayer>();
	titleLayer_->Initialize();
	titleLayer_->SetIsCollision(true);
#pragma endregion

#pragma region 設定レイヤー
	settingLayer_ = std::make_unique<SettingLayer>();
	settingLayer_->Initialize();
	settingLayer_->SetIsCollision(false);
#pragma endregion
}

void TitleScene::Update()
{
	// カメラ回転
	CameraRota();

	// タイトルレイヤー
	if (layerState_ == LayerState::TITLE) titleLayer_->Update();

	// 設定レイヤー	
	else if (layerState_ == LayerState::SETTING) settingLayer_->Update();

	// 衝突処理2D
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	stage_->Draw();

	PipelineManager::PreDraw("Sprite");

	// タイトルレイヤー
	if (layerState_ == LayerState::TITLE) titleLayer_->Draw();

	// 設定レイヤー	
	else if (layerState_ == LayerState::SETTING) settingLayer_->Draw();
}

void TitleScene::OnCollision()
{
	// 衝突全チェック
	colMgr2D_->CheckAllCollision();

	// タイトルレイヤー
	if (layerState_ == LayerState::TITLE) titleLayer_->OnCollision(selectNum_);

	// 設定レイヤー	
	else if (layerState_ == LayerState::SETTING) settingLayer_->OnCollision(selectNum_);

	if (selectNum_ == SelectNum::START)
	{
		SceneManager::GetInstance()->SceneTransition(GAME);
	}

	else if (selectNum_ == SelectNum::SETTING)
	{
		layerState_ = LayerState::SETTING;
		titleLayer_->SetIsCollision(false);
		settingLayer_->SetIsCollision(true);
	}

	else if (selectNum_ == SelectNum::END)
	{
		SceneManager::GetInstance()->SetIsEnd(true);
	}

	else if (selectNum_ == SelectNum::RETURN)
	{
		layerState_ = LayerState::TITLE;
		titleLayer_->SetIsCollision(true);
		settingLayer_->SetIsCollision(false);
	}
}

void TitleScene::MatUpdate()
{
	// カメラ更新
	camera_->Update();

	// ステージ
	stage_->MatUpdate();

	// タイトルレイヤー
	if (layerState_ == LayerState::TITLE) titleLayer_->MatUpdate();

	// 設定レイヤー	
	else if (layerState_ == LayerState::SETTING) settingLayer_->MatUpdate();


}

void TitleScene::CameraRota()
{
	static float angle = 0.0f;
	angle += 0.1f;

	Vector3 cameraPos = {
		sinf(Util::Degree2Radian(angle)),
		0.0f,
		cosf(Util::Degree2Radian(angle))
	};

	cameraPos.normalize();

	cameraPos *= 90.0f;
	cameraPos.y = 10.0f;

	camera_->SetEye(cameraPos);
}