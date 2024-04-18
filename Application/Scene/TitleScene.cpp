#include "TitleScene.h"
#include "Texture.h"
#include "PipelineManager.h"
#include "SceneManager.h"
#include "CollisionAttribute.h"
#include "Util.h"
#include "Easing.h"
#include "Instancing3D.h"
#include "WinAPI.h"

#include "TitleLayer.h"
#include "GamePlayLayer.h"
#include "SettingLayer.h"
#include "AudioLayer.h"

#include <imgui_impl_DX12.h>

TitleScene::TitleScene(IScene* sceneIf) : BaseScene(sceneIf) {}

void TitleScene::Initialize()
{

#pragma region インスタンス取得
	colMgr2D_ = CollisionManager2D::GetInstance();// 衝突マネージャー2D
	mouse_ = Mouse::GetInstance();
	BoxButton::SetCollisionManager2D(colMgr2D_);
#pragma endregion

#pragma region ライトグループ
	lightGroup_ = LightGroup::GetInstance();
	Object3D::SetLightGroup(lightGroup_);
	Instancing3D::SetLightGroup(lightGroup_);

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
	Instancing3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ステージ
	stage_ = std::make_unique<Stage>();
	stage_->Initialize();
	stage_->Load("Resources/StageJson/stage1.json", true);
#pragma endregion

#pragma region コライダー
	cMouse_ = std::make_unique<PointCollider>();
	cMouse_->SetAttribute(COL_POINT);
	colMgr2D_->AddCollider(cMouse_.get());
#pragma endregion

	// レイヤーの生成
	layers_.emplace_back(std::make_unique<TitleLayer>(colMgr2D_));
	layers_.emplace_back(std::make_unique<SettingLayer>(colMgr2D_));
	layers_.emplace_back(std::make_unique<GamePlayLayer>(colMgr2D_));
	layers_.emplace_back(std::make_unique<AudioLayer>(colMgr2D_));

	// レイヤーの初期化
	for (auto& it : layers_) it->Initialize();
	layers_[0]->SetValid(true);
	layers_[1]->SetValid(false);
	layers_[2]->SetValid(false);
	layers_[3]->SetValid(false);
}

void TitleScene::Update()
{
	// コライダーとマウスの座標をリンク
	cMouse_->SetOffset(mouse_->MousePos());

	for (auto& it : layers_) it->Update();

	// カメラ回転
	CameraRota();

	// 衝突処理2D
	Collision();

	// 行列更新処理
	MatUpdate();
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Object3D");
	stage_->Draw();

	PipelineManager::PreDraw("Sprite");
	for (auto& it : layers_) it->Draw();
}

void TitleScene::Finalize()
{
	for (auto& it : layers_) it->Finalize();
	colMgr2D_->RemoveCollider(cMouse_.get());
	lightGroup_->RemoveDirLight(dirLight_.get());
	CollisionManager3D::GetInstance()->Release();
}

void TitleScene::Collision()
{
	// 衝突全チェック
	colMgr2D_->CheckAllCollision();
	for (auto& it : layers_) it->Collision();

	if (!mouse_->TriggerMouseButton(MouseButton::M_LEFT)) return;
	if (cMouse_->GetHitCollider() == nullptr) return;
	int32_t tag = cMouse_->GetHitCollider()->GetTag();

	if (tag == (int32_t)ButtonAttribute::START)
	{
		sceneIf_->ChangeScene(Scene::GAME);
		WinAPI::GetInstance()->DisplayCursor(false);
		WinAPI::GetInstance()->SetClipCursor(true);
	}

	else if (tag == (int32_t)ButtonAttribute::SETTING)
	{
		layers_[0]->SetValid(false);
		layers_[1]->SetValid(true);
		layers_[2]->SetValid(true);
		layers_[3]->SetValid(false);
	}

	else if (tag == (int32_t)ButtonAttribute::END)
	{
		sceneIf_->GameEnd();
	}

	else if (tag == (int32_t)ButtonAttribute::GAMEPLAY)
	{
		layers_[2]->SetValid(true);
		layers_[3]->SetValid(false);
	}

	else if (tag == (int32_t)ButtonAttribute::AUDIO)
	{
		layers_[2]->SetValid(false);
		layers_[3]->SetValid(true);
	}

	else if (tag == (int32_t)ButtonAttribute::RETURN)
	{
		layers_[0]->SetValid(true);
		layers_[1]->SetValid(false);
		layers_[2]->SetValid(false);
		layers_[3]->SetValid(false);
	}
}

void TitleScene::MatUpdate()
{
	// カメラ更新
	camera_->Update();

	// ステージ
	stage_->MatUpdate();

	for (auto& it : layers_) it->MatUpdate();
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

	cameraPos *= 50.0f;
	cameraPos.y = 10.0f;

	camera_->SetEye(cameraPos);
}