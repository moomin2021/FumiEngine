#include "TitleScene.h"
#include "Texture.h"
#include "PipelineManager.h"
#include "SceneManager.h"
#include "CollisionAttribute.h"
#include "Util.h"
#include "Easing.h"
#include "Instancing3D.h"

#include <imgui_impl_DX12.h>

TitleScene::TitleScene(IScene* sceneIf) : BaseScene(sceneIf) {}

void TitleScene::Initialize()
{

	uint16_t leg = 0b0000000000000001;
	uint16_t obj = 0b0000000010000111;

	bool result = leg == obj;

	result = result;

	result = !(leg & obj);

	result = result;

#pragma region インスタンス取得
	colMgr2D_ = CollisionManager2D::GetInstance();// 衝突マネージャー2D
	mouse_ = Mouse::GetInstance();
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

#pragma region タイトルレイヤー
	titleLayer_ = std::make_unique<TitleLayer>();
	titleLayer_->Initialize();
#pragma endregion

#pragma region 設定レイヤー
	settingLayer_ = std::make_unique<SettingLayer>();
	settingLayer_->Initialize();
	settingLayer_->SetIsDisplay(false);
#pragma endregion

#pragma region ゲームプレイレイヤー
	gamePlayLayer_ = std::make_unique<GamePlayLayer>();
	gamePlayLayer_->Initialize();
	gamePlayLayer_->SetIsDisplay(false);
#pragma endregion

#pragma region オーディオレイヤー
	audioLayer_ = std::make_unique<AudioLayer>();
	audioLayer_->Initialize();
	audioLayer_->SetIsDisplay(false);
#pragma endregion
}

void TitleScene::Update()
{
	// コライダーとマウスの座標をリンク
	cMouse_->SetOffset(mouse_->MousePos());

	// レイヤー
	titleLayer_->Update();
	settingLayer_->Update();
	gamePlayLayer_->Update();
	audioLayer_->Update();

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

	// レイヤー
	titleLayer_->Draw();
	settingLayer_->Draw();
	gamePlayLayer_->Draw();
	audioLayer_->Draw();
}

void TitleScene::Finalize()
{
	colMgr2D_->RemoveCollider(cMouse_.get());
	lightGroup_->RemoveDirLight(dirLight_.get());
}

void TitleScene::Collision()
{
	// 衝突全チェック
	colMgr2D_->CheckAllCollision();

	// レイヤー
	titleLayer_->OnCollision();
	settingLayer_->OnCollision();
	gamePlayLayer_->OnCollision();
	audioLayer_->OnCollision();

#pragma region 左クリックを押したら
	// 衝突していなかったらこれ以降の処理を飛ばす
	if (mouse_->TriggerMouseButton(M_LEFT) == false) return;

	// 取得するボタンの属性
	ButtonAttribute buttonAttr = ButtonAttribute::NONE;

	// マウスのコライダーから衝突しているコライダーのタグを属性に変換して取得
	if (cMouse_->GetHitCollider() != nullptr)
	{
		buttonAttr = (ButtonAttribute)cMouse_->GetHitCollider()->GetTag();
	}

	// スタート
	if (buttonAttr == ButtonAttribute::START)
	{
		sceneIf_->ChangeScene(Scene::GAME);
	}

	// 設定
	else if (buttonAttr == ButtonAttribute::SETTING)
	{
		titleLayer_->SetIsDisplay(false);
		settingLayer_->SetIsDisplay(true);
		gamePlayLayer_->SetIsDisplay(true);
		audioLayer_->SetIsDisplay(false);
	}

	// テストシーン
	else if (buttonAttr == ButtonAttribute::TEST)
	{
		sceneIf_->ChangeScene(Scene::TEST);
	}

	// ゲーム終了
	else if (buttonAttr == ButtonAttribute::END)
	{
		sceneIf_->GameEnd();
	}

	// ゲームプレイ
	else if (buttonAttr == ButtonAttribute::GAMEPLAY)
	{
		gamePlayLayer_->SetIsDisplay(true);
		audioLayer_->SetIsDisplay(false);
	}

	// オーディオ
	else if (buttonAttr == ButtonAttribute::AUDIO)
	{
		gamePlayLayer_->SetIsDisplay(false);
		audioLayer_->SetIsDisplay(true);
	}

	// 戻る
	else if (buttonAttr == ButtonAttribute::RETURN)
	{
		titleLayer_->SetIsDisplay(true);
		settingLayer_->SetIsDisplay(false);
		gamePlayLayer_->SetIsDisplay(false);
		audioLayer_->SetIsDisplay(false);
	}
#pragma endregion
}

void TitleScene::MatUpdate()
{
	// カメラ更新
	camera_->Update();

	// ステージ
	stage_->MatUpdate();

	// レイヤー
	titleLayer_->MatUpdate();
	settingLayer_->MatUpdate();
	gamePlayLayer_->MatUpdate();
	audioLayer_->MatUpdate();
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