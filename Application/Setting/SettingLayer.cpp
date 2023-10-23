#include "SettingLayer.h"
#include "Texture.h"
#include "CollisionAttribute.h"
#include "Util.h"
#include "Easing.h"

SettingLayer::~SettingLayer()
{
	
}

void SettingLayer::Initialize()
{
#pragma region インスタンス
	mouse_ = Mouse::GetInstance();
	colMgr2D_ = CollisionManager2D::GetInstance();
#pragma endregion

#pragma region ツリーボタン
	buttons_.resize(2);
	BoxButton::SetCollisionManager2D(CollisionManager2D::GetInstance());
	for (uint16_t i = 0; i < buttons_.size(); i++)
	{
		buttons_[i] = std::make_unique<BoxButton>();
		buttons_[i]->Initialize(uint16_t(SelectNum::GAMEPLAY) + i, { 300.0f + (i * 280.0f), 101.0f }, { 260.0f, 44.0f },
			LoadTexture("Resources/treeButton.png"),
			LoadTexture("Resources/treeText" + std::string(std::to_string(i)) + std::string(".png")));
	}
#pragma endregion

#pragma region 戻るボタン
	gFrameReturn_ = LoadTexture("Resources/returnText.png");

	returnButton_ = std::make_unique<BoxButton>();
	returnButton_->Initialize((int32_t)SelectNum::RETURN, { 200.0f, 1000.0f }, {260.0f, 44.0f},
		LoadTexture("Resources/treeButton.png"),
		LoadTexture("Resources/returnText.png"));
#pragma endregion

#pragma region フレーム関連
	// 画像読み込み
	frameHandle_ = LoadTexture("Resources/treeButtonCursorFrame.png");

	// フレーム生成
	hitFrame_ = std::make_unique<HitFrame>();
	hitFrame_->Initialize();
	hitFrame_->SetFrame(frameSize_, frameHandle_);
#pragma endregion

#pragma region 設定の背景画像
	gBackBox_ = LoadTexture("Resources/backBox.png");

	sBackBox_ = std::make_unique<Sprite>();
	sBackBox_->SetPosition({ 160.0f, 176.0f });
	sBackBox_->SetSize({ 768.0f, 746.0f });
#pragma endregion
}

void SettingLayer::Update()
{
	// ボタン
	for (auto& it : buttons_) it->Update();

	// フレーム
	hitFrame_->Update();

	// 戻るボタン
	returnButton_->Update();
}

void SettingLayer::Draw()
{
	// ボタン
	for (auto& it : buttons_) it->Draw();

	// フレーム
	hitFrame_->Draw();

	// 設定の背景画像
	sBackBox_->Draw(gBackBox_);

	// 戻るボタン
	returnButton_->Draw();
}

void SettingLayer::OnCollision(SelectNum& selectNum)
{
	// ボタン
	for (auto& it : buttons_) it->OnCollision();

	// フレーム
	hitFrame_->OnCollision(selectNum);

	// 戻るボタン
	returnButton_->OnCollision();
}

void SettingLayer::MatUpdate()
{
	// ボタン
	for (auto& it : buttons_) it->MatUpdate();

	// フレーム
	hitFrame_->MatUpdate();

	// 設定の背景画像
	sBackBox_->MatUpdate();

	// 戻るボタン
	returnButton_->MatUpdate();
}

void SettingLayer::SetIsCollision(bool frag)
{
	for (auto& it : buttons_) it->SetIsCollision(frag);
	hitFrame_->SetIsCollision(frag);
	returnButton_->SetIsCollision(frag);
}