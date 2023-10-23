#include "TitleLayer.h"
#include "CollisionManager2D.h"
#include "Texture.h"

TitleLayer::~TitleLayer()
{
}

void TitleLayer::Initialize()
{
#pragma region ボタン
	buttons_.resize(3);
	BoxButton::SetCollisionManager2D(CollisionManager2D::GetInstance());
	for (uint16_t i = 0; i < buttons_.size(); i++)
	{
		buttons_[i] = std::make_unique<BoxButton>();
		buttons_[i]->Initialize(i, { 250.0f, 525.0f + (i * 50.0f) }, { 310.0f, 40.0f },
			LoadTexture("Resources/titleSelectButton.png"),
			LoadTexture("resources/titleSelectText" + std::string(std::to_string(i)) + std::string(".png")));
	}
#pragma endregion

#pragma region フレーム関連
	// 画像読み込み
	frameHandle_ = LoadTexture("Resources/titleSelectButtonFrame.png");

	// フレーム生成
	hitFrame_ = std::make_unique<HitFrame>();
	hitFrame_->Initialize();
	hitFrame_->SetFrame(frameSize_, frameHandle_);
#pragma endregion
}

void TitleLayer::Update()
{
	// ボタン
	for (auto& it : buttons_) it->Update();

	// フレーム
	hitFrame_->Update();
}

void TitleLayer::Draw()
{
	// ボタン
	for (auto& it : buttons_) it->Draw();

	// フレーム
	hitFrame_->Draw();
}

void TitleLayer::OnCollision(SelectNum& selectNum)
{
	// ボタン
	for (auto& it : buttons_) it->OnCollision();

	// フレーム
	hitFrame_->OnCollision(selectNum);
}

void TitleLayer::MatUpdate()
{
	// ボタン
	for (auto& it : buttons_) it->MatUpdate();

	// フレーム
	hitFrame_->MatUpdate();
}

void TitleLayer::SetIsCollision(bool frag)
{
	for (auto& it : buttons_) it->SetIsCollision(frag);
	hitFrame_->SetIsCollision(frag);
}