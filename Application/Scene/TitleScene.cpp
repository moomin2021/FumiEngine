#include "TitleScene.h"
#include "Texture.h"
#include "PipelineManager.h"
#include "SceneManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
#pragma region インスタンス取得
	key_ = Key::GetInstance();// マウス
#pragma endregion

#pragma region スプライト
	sTitle_ = std::make_unique<Sprite>();
	sTitle_->SetSize(Vector2{1920.0f, 1080.0f});
#pragma endregion

#pragma region テクスチャハンドル
	hTitle_ = LoadTexture("Resources/Title.png");
#pragma endregion
}

void TitleScene::Update()
{
	if (key_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->SceneTransition(GAME);
	}
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Sprite");

	// タイトルを描画
	sTitle_->Draw(hTitle_);
}

void TitleScene::OnCollision()
{
}

void TitleScene::ObjUpdate()
{
}
