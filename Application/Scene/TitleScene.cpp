#include "TitleScene.h"
#include "PipelineManager.h"
#include "SceneManager.h"
#include "Texture.h"
#include "Util.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize()
{
	// インスタンス取得
	key_ = Key::GetInstance();

	// カメラ設定
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 0.0f, -5.0f });

	// スプライト生成
	sTitle_ = std::make_unique<Sprite>();
	sTitle_->SetSize({ 1920.0f, 1080.0f });

	// 画像読み込み
	titleHandle_ = LoadTexture("Resources/title.png");

	bgm_ = Sound::LoadWave("Resources/Sound/titleBGM.wav");
	Sound::Play(bgm_);
}

void TitleScene::Update()
{
	// カメラ更新
	camera_->Update();

	if (key_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->SceneTransition(SCENE::SCENE1);
	}
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Sprite");

	// スプライト描画
	sTitle_->Draw(titleHandle_);
}