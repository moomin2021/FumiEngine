#include "TestScene.h"
#include "CollisionManager3D.h"
#include "WinAPI.h"
#include "Texture.h"

#include "PipelineManager.h"

#include <set>
#include <imgui_impl_DX12.h>

TestScene::TestScene(IScene* sceneIf) : BaseScene(sceneIf) {}

void TestScene::Initialize()
{
#pragma region カーソルの設定
	WinAPI::GetInstance()->DisplayCursor(false);
	WinAPI::GetInstance()->SetClipCursor(true);
#pragma endregion

#pragma region インスタンス
	key_ = Key::GetInstance();
	lightGroup_ = LightGroup::GetInstance();
	Object3D::SetLightGroup(lightGroup_);
	Instancing3D::SetLightGroup(lightGroup_);
#pragma endregion

#pragma region ライトグループ
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region ステージクラス
	stage_ = std::make_unique<Stage>();
	stage_->Initialize();
#pragma endregion

#pragma region プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize({ 0.0f, 5.0f, 0.0f });

	playerUI_ = std::make_unique<PlayerUI>(player_.get(), player_->GetWeapon());
	playerUI_->Initialize();
	playerUI_->SetPlayer(player_.get());
#pragma endregion

#pragma region カメラマネージャー
	cameraMgr_ = std::make_unique<CameraManager>();
	cameraMgr_->Initialize(player_->GetCamera());
#pragma endregion

#pragma region デバックカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(player_.get());
#pragma endregion

	// ステージ読み込み
	stage_->Load("Resources/StageJson/test.json", false, true);

#pragma region ゲームUI
	sGameUI_ = std::make_unique<Sprite>();
	sGameUI_->SetSize({ 1920.0f, 1080.0f });

	gGameUI_ = LoadTexture("Sprite/GameUI.png");

	sObjectiveText_ = std::make_unique<Sprite>();
	sObjectiveText_->SetAnchorPoint({ 0.5f, 0.5f });
	sObjectiveText_->SetPosition({ 1700.0f, 300.0f });
	sObjectiveText_->SetSize({ 400.0f, 200.0f });

	gObjectiveText_ = LoadTexture("Sprite/objectiveText.png");
#pragma endregion
}

void TestScene::Update()
{
	// プレイヤー
	player_->Update();
	playerUI_->Update();

	debugCamera_->Update();

	// 衝突時処理
	Collision();

	// 行列更新処理
	MatUpdate();

	// デバック
	Debug();
}

void TestScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// ステージクラス
	stage_->Draw();

	// プレイヤー
	player_->Draw();

	PipelineManager::PreDraw("Sprite");

	// プレイヤー
	playerUI_->Draw();

	sGameUI_->Draw(gGameUI_);

	sObjectiveText_->Draw(gObjectiveText_);
}

void TestScene::Finalize()
{
	lightGroup_->RemoveDirLight(dirLight_.get());
}

void TestScene::Debug()
{
	if (key_->TriggerKey(DIK_0))
	{
		if (isDebug_)
		{
			isDebug_ = false;
			WinAPI::GetInstance()->DisplayCursor(false);
			WinAPI::GetInstance()->SetClipCursor(true);

			cameraMgr_->ChangeCamera(player_->GetCamera());
		}

		else
		{
			isDebug_ = true;
			WinAPI::GetInstance()->DisplayCursor(true);
			WinAPI::GetInstance()->SetClipCursor(false);

			cameraMgr_->ChangeCamera(debugCamera_->GetCamera());
		}
	}

	stage_->Debug(isDebug_);

	if (isDebug_ == false) return;

	ImGui::Begin("DeltaTime");
	ImGui::End();

	debugCamera_->Debug(isDebug_);
}

void TestScene::Collision()
{
	// 衝突判定をとる
	CollisionManager3D::GetInstance()->CheckAllCollision();

	// プレイヤー
	player_->Collision();

	playerUI_->OnCollision();
}

void TestScene::MatUpdate()
{
	cameraMgr_->MatUpdate();

	// プレイヤー
	player_->MatUpdate();
	playerUI_->MatUpdate();

	// ステージクラス
	stage_->MatUpdate();

	debugCamera_->MatUpdate();

	sGameUI_->MatUpdate();
	sObjectiveText_->MatUpdate();
}
