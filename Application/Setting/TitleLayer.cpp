#include "TitleLayer.h"
#include "CollisionManager2D.h"
#include "Texture.h"

TitleLayer::~TitleLayer()
{
}

void TitleLayer::Initialize()
{
#pragma region ボタン
	buttons_.resize(4);
	BoxButton::SetCollisionManager2D(CollisionManager2D::GetInstance());
	for (uint16_t i = 0; i < buttons_.size(); i++)
	{
		buttons_[i] = std::make_unique<BoxButton>();
		buttons_[i]->Initialize(i, { 250.0f, 525.0f + (i * 50.0f) }, { 310.0f, 40.0f }, {324.0f, 54.0f},
			LoadTexture("Resources/titleSelectButton.png"),
			LoadTexture("resources/titleSelectText" + std::string(std::to_string(i)) + std::string(".png")),
			LoadTexture("Resources/titleSelectButtonFrame.png"));
	}
#pragma endregion

#pragma region タイトル
	sTitle_ = std::make_unique<Sprite>();
	sTitle_->SetSize({ 1920.0f, 1080.0f });

	gTitle_ = LoadTexture("Resources/title.png");
#pragma endregion
}

void TitleLayer::Update()
{
	// ボタン
	for (auto& it : buttons_) it->Update();
}

void TitleLayer::Draw()
{
	if (isDraw_ == false) return;

	// ボタン
	for (auto& it : buttons_) it->Draw();

	// タイトル
	sTitle_->Draw(gTitle_);
}

void TitleLayer::OnCollision()
{
	// ボタン
	for (auto& it : buttons_) it->OnCollision();
}

void TitleLayer::MatUpdate()
{
	// ボタン
	for (auto& it : buttons_) it->MatUpdate();

	// タイトル
	sTitle_->MatUpdate();
}

void TitleLayer::SetIsDisplay(bool frag)
{
	for (auto& it : buttons_) it->SetIsCollision(frag);
	isDraw_ = frag;
}