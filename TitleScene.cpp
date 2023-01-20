#include "TitleScene.h"
#include "FumiEngine.h"
#include "SceneManager.h"

TitleScene::TitleScene() :
	key_(nullptr),
	titleS_(nullptr),
	camera_(nullptr)
{
}

TitleScene::~TitleScene()
{
	delete camera_;
}

void TitleScene::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	titleS_ = std::make_unique<Sprite>();
	titleS_->scale = { 19.20f, 10.80f };

	titleImage_ = LoadTexture("Resources/title.png");

	spaceStartText_ = std::make_unique<Sprite>();
	spaceStartText_->scale = { 19.20f, 10.80f };

	spaceStartTextH_ = LoadTexture("Resources/spaceStartText.png");

	titleS_->Update();

	spaceStartText_->Update();
}

void TitleScene::Update()
{
	// カメラの更新
	camera_->Update();

	titleS_->Update();

	spaceStartText_->Update();

	if (key_->TriggerKey(DIK_SPACE)) {
		SceneManager::ChangeScene(SCENE::GAME);
	}
}

void TitleScene::Draw()
{
	Sprite::PreDraw();

	titleS_->Draw(titleImage_);
	spaceStartText_->Draw(spaceStartTextH_);
}
