#include "GameScene.h"
#include "CollisionManager3D.h"
#include "WinAPI.h"
#include "Texture.h"

#include "PipelineManager.h"

#include <set>
#include <imgui_impl_DX12.h>

GameScene::GameScene(IScene* sceneIf) : BaseScene(sceneIf) {}

void GameScene::Initialize()
{
#pragma region カーソルの設定
	WinAPI::GetInstance()->DisplayCursor(true);
	WinAPI::GetInstance()->SetClipCursor(false);
#pragma endregion

#pragma region インスタンス
	key_ = Key::GetInstance();
	lightGroup_ = LightGroup::GetInstance();
	deltaTime_ = DeltaTime::GetInstance();
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
#pragma endregion

#pragma region プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize({ 0.0f, 0.0f, 0.0f });

	playerUI_ = std::make_unique<PlayerUI>(player_.get(), player_->GetWeapon());
	playerUI_->Initialize();
	playerUI_->SetPlayer(player_.get());
#pragma endregion

// エネミーマネージャー
	enemyMgr_ = std::make_unique<EnemyManager>();

#pragma region カメラマネージャー
	cameraMgr_ = std::make_unique<CameraManager>();
	cameraMgr_->Initialize(player_->GetCamera());
#pragma endregion

#pragma region デバックカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(player_.get());
#pragma endregion

	// 設定
	stage_->SetEnemyManager(enemyMgr_.get());
	enemyMgr_->SetLightGroup(lightGroup_);
	enemyMgr_->SetNavMesh(stage_->GetNavMesh());
	enemyMgr_->SetPlayer(player_.get());

	// 初期化
	stage_->Initialize();
	enemyMgr_->Initialize();

	// ステージ読み込み
	stage_->Load("Resources/StageJson/test.json");

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

void GameScene::Update()
{
	// プレイヤー
	player_->Update();
	playerUI_->Update();

	enemyMgr_->Update();

	debugCamera_->Update();

	// 衝突時処理
	Collision();

	// 行列更新処理
	MatUpdate();

	// デバック
	if (!isDebug_) Debug();

	// プレイヤーが死んだらシーンを切り替える
	if (player_->GetIsAlive() == false) sceneIf_->ChangeScene(Scene::RESULT);
}

void GameScene::Draw()
{
	PipelineManager::PreDraw("Object3D");
	stage_->Draw();
	player_->Draw();
	enemyMgr_->Draw();

	PipelineManager::PreDraw("Sprite");
	playerUI_->Draw();
	sGameUI_->Draw(gGameUI_);
	sObjectiveText_->Draw(gObjectiveText_);
}

void GameScene::Finalize()
{
	lightGroup_->RemoveDirLight(dirLight_.get());
	enemyMgr_->Finalize();
}

void GameScene::Debug()
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

	static float timeSpd = 1.0f;

	if (key_->TriggerKey(DIK_UP)) timeSpd += 0.1f;
	if (key_->TriggerKey(DIK_DOWN)) timeSpd -= 0.1f;

	deltaTime_->SetTimeSpeed(timeSpd);

	stage_->Debug(isDebug_);

	ImGui::Begin("DeltaTime");
	ImGui::Text("deltaTime = %f", deltaTime_->GetDeltaTime());
	ImGui::Text("GameTime = %f", deltaTime_->GetGameTime());
	ImGui::Text("GameSpeed = %f", deltaTime_->GetTimeSpeed());
	ImGui::End();

	if (isDebug_ == false) return;

	debugCamera_->Debug();
}

void GameScene::Collision()
{
	// 衝突判定をとる
	CollisionManager3D::GetInstance()->CheckAllCollision();
	player_->Collision();
	playerUI_->OnCollision();
	enemyMgr_->Collision();
}

void GameScene::MatUpdate()
{
	cameraMgr_->MatUpdate();
	player_->MatUpdate();
	playerUI_->MatUpdate();
	enemyMgr_->MatUpdate();
	stage_->MatUpdate();
	debugCamera_->MatUpdate();
	sGameUI_->MatUpdate();
	sObjectiveText_->MatUpdate();
}
