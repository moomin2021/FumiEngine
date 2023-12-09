#include "GameScene.h"
#include "CollisionManager.h"
#include "WinAPI.h"
#include "Texture.h"

#include "PipelineManager.h"

#include <set>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	lightGroup_->RemoveDirLight(dirLight_.get());
}

void GameScene::Initialize()
{
#pragma region カーソルの設定
	WinAPI::GetInstance()->DisplayCursor(false);
	WinAPI::GetInstance()->SetClipCursor(true);
#pragma endregion

#pragma region インスタンス
	key_ = Key::GetInstance();
	lightGroup_ = LightGroup::GetInstance();
	Object3D::SetLightGroup(lightGroup_);
	EnemyCore::SetLightGroup(lightGroup_);
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
	player_->Initialize();
#pragma endregion

#pragma region エネミーマネージャー
	// エネミーマネージャー生成
	enemyMgr_ = std::make_unique<EnemyManager>();
	enemyMgr_->Initialize();
	enemyMgr_->SetPlayer(player_.get());
	stage_->SetEnemyManager(enemyMgr_.get());
	player_->SetEnemyManager(enemyMgr_.get());
#pragma endregion

#pragma region モデル
	mCube_ = std::make_unique<Model>("cube");
#pragma endregion

#pragma region 3軸を示すオブジェクト
	oAxis_.resize(3);
	oAxis_[0] = std::make_unique<Object3D>(mCube_.get());
	oAxis_[1] = std::make_unique<Object3D>(mCube_.get());
	oAxis_[2] = std::make_unique<Object3D>(mCube_.get());

	oAxis_[0]->SetPosition({ 5.0f, 0.0f, 0.0f });
	oAxis_[1]->SetPosition({ 0.0f, 5.0f, 0.0f });
	oAxis_[2]->SetPosition({ 0.0f, 0.0f, 5.0f });

	oAxis_[0]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	oAxis_[1]->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	oAxis_[2]->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
#pragma endregion

	// ステージ読み込み
	stage_->Load("Resources/StageJson/stage1.json");

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

	// エネミーマネージャー
	enemyMgr_->Update();

	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();

	// デバック
	Debug();

	enemyMgr_->CheckSceneChange();
	player_->CheckSceneChange();
}

void GameScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// ステージクラス
	stage_->Draw();

	// プレイヤー
	player_->Draw3D();

	// エネミーマネージャー
	enemyMgr_->Draw();

	// 3軸を示すオブジェクト
	if (isDebug_)
	{
		for (auto& it : oAxis_)
		{
			it->Draw();
		}
	}

	PipelineManager::PreDraw("Sprite");

	// プレイヤー
	player_->DrawFront2D();

	sGameUI_->Draw(gGameUI_);

	sObjectiveText_->Draw(gObjectiveText_);
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
		}

		else
		{
			isDebug_ = true;
			WinAPI::GetInstance()->DisplayCursor(true);
			WinAPI::GetInstance()->SetClipCursor(false);
		}
	}

	if (isDebug_ == false) return;

	player_->Debug();

	// エネミーマネージャー
	enemyMgr_->Debug();
}

void GameScene::OnCollision()
{
	// 衝突判定をとる
	CollisionManager::GetInstance()->CheckAllCollision();

	// プレイヤー
	player_->OnCollision();

	// エネミーマネージャー
	enemyMgr_->OnCollision();
}

void GameScene::MatUpdate()
{
	// プレイヤー
	player_->MatUpdate();

	// エネミーマネージャー
	enemyMgr_->MatUpdate();

	// ステージクラス
	stage_->MatUpdate();

	// 3軸を示すオブジェクト
	for (auto& it : oAxis_) {
		it->MatUpdate();
	}

	sGameUI_->MatUpdate();
	sObjectiveText_->MatUpdate();
}
