#include "SettingLayer.h"
#include "Texture.h"

void SettingLayer::Initialize()
{
#pragma region ボタン
	buttons_.resize(3);
	BoxButton::SetCollisionManager2D(CollisionManager2D::GetInstance());
	for (uint16_t i = 0; i < buttons_.size(); i++)
	{
		if (i == 2)
		{
			buttons_[i] = std::make_unique<BoxButton>();
			buttons_[i]->Initialize(uint16_t(ButtonAttribute::GAMEPLAY) + i, { 200.0f, 1000.0f }, { 260.0f, 44.0f }, { 272.0f, 56.0f },
				LoadTexture("Sprite/treeButton.png"),
				LoadTexture("Sprite/returnText.png"),
				LoadTexture("Sprite/treeButtonCursorFrame.png"));
		}

		else
		{
			buttons_[i] = std::make_unique<BoxButton>();
			buttons_[i]->Initialize(uint16_t(ButtonAttribute::GAMEPLAY) + i, { 300.0f + (i * 280.0f), 101.0f }, { 260.0f, 44.0f }, { 272.0f, 56.0f },
				LoadTexture("Sprite/treeButton.png"),
				LoadTexture("Sprite/treeText" + std::string(std::to_string(i)) + std::string(".png")),
				LoadTexture("Sprite/treeButtonCursorFrame.png"));
		}
	}
#pragma endregion

#pragma region 設定一覧の背景画像
	sSettingBackground_ = std::make_unique<Sprite>();
	sSettingBackground_->SetPosition({ 160.0f, 176.0f });
	sSettingBackground_->SetSize({ 768.0f, 746.0f });

	gSettingBackground_ = LoadTexture("Sprite/backBox.png");
#pragma endregion
}

void SettingLayer::Update()
{
	for (auto& it : buttons_) it->Update();
	for (auto& it : buttons_) it->SetIsCollision(isValid_);
}

void SettingLayer::Draw()
{
	if (isValid_ == false) return;
	for (auto& it : buttons_) it->Draw();
	sSettingBackground_->Draw(gSettingBackground_);
}

void SettingLayer::Collision()
{
	for (auto& it : buttons_) it->OnCollision();
}

void SettingLayer::MatUpdate()
{
	for (auto& it : buttons_) it->MatUpdate();
	sSettingBackground_->MatUpdate();
}

void SettingLayer::Finalize()
{

}