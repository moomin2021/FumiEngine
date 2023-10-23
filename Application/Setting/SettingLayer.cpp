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

#pragma region ボタン
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

#pragma region フレーム関連
	// 画像読み込み
	frameHandle_ = LoadTexture("Resources/treeButtonCursorFrame.png");

	// フレーム生成
	hitFrame_ = std::make_unique<HitFrame>();
	hitFrame_->Initialize();
	hitFrame_->SetFrame(frameSize_, frameHandle_);
#pragma endregion
}

void SettingLayer::Update()
{
	// ボタン
	for (auto& it : buttons_) it->Update();

	// フレーム
	hitFrame_->Update();
}

void SettingLayer::Draw()
{
	// ボタン
	for (auto& it : buttons_) it->Draw();

	// フレーム
	hitFrame_->Draw();
}

void SettingLayer::OnCollision(SelectNum& selectNum)
{
	// ボタン
	for (auto& it : buttons_) it->OnCollision();

	// フレーム
	hitFrame_->OnCollision(selectNum);
}

void SettingLayer::MatUpdate()
{
	// ボタン
	for (auto& it : buttons_) it->MatUpdate();

	// フレーム
	hitFrame_->MatUpdate();
}

void SettingLayer::SetIsCollision(bool frag)
{
	for (auto& it : buttons_) it->SetIsCollision(frag);
	hitFrame_->SetIsCollision(frag);
}