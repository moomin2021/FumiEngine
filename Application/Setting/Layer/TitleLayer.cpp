#include "TitleLayer.h"
#include "Texture.h"

void TitleLayer::Initialize()
{
	// ボタンの生成＆設定
	buttons_.resize(3);
	for (uint16_t i = 0; i < buttons_.size(); i++)
	{
		buttons_[i] = std::make_unique<BoxButton>();
		buttons_[i]->Initialize(i, { 250.0f, 525.0f + (i * 50.0f) }, { 310.0f, 40.0f }, { 324.0f, 54.0f },
			LoadTexture("Sprite/titleSelectButton.png"),
			LoadTexture("Sprite/titleSelectText" + std::string(std::to_string(i)) + std::string(".png")),
			LoadTexture("Sprite/titleSelectButtonFrame.png"));
	}

	// スプライトの生成&設定
	titleS_ = std::make_unique<Sprite>();
	titleS_->SetSize({ 1920.0f, 1080.0f });

	// テクスチャ取得
	titleH_ = LoadTexture("Sprite/title.png");
}

void TitleLayer::Update()
{
	for (auto& it : buttons_) it->Update();
	for (auto& it : buttons_) it->SetIsCollision(isValid_);
}

void TitleLayer::Draw()
{
	if (isValid_ == false) return;
	for (auto& it : buttons_) it->Draw();
}

void TitleLayer::Collision()
{
	for (auto& it : buttons_) it->OnCollision();
}

void TitleLayer::MatUpdate()
{
	for (auto& it : buttons_) it->MatUpdate();
}

void TitleLayer::Finalize()
{

}