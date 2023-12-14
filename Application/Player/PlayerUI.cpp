#include "PlayerUI.h"
#include "Texture.h"
#include "WinAPI.h"

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Initialize()
{
	// ウィンドウサイズ
	Vector2 winSize = { (float)WinAPI::GetInstance()->GetWidth(), (float)WinAPI::GetInstance()->GetHeight()};

#pragma region スプライト
	hpBarS_ = std::make_unique<Sprite>();
	hpBarS_->SetAnchorPoint({ 0.0f, 0.0f });
	hpBarS_->SetPosition({ 102.0f, winSize.y - 98.0f });
	hpBarS_->SetSize({ 434.0f, 34.0f });

	hpFrameS_ = std::make_unique<Sprite>();
	hpFrameS_->SetAnchorPoint({ 0.0f, 0.0f });
	hpFrameS_->SetPosition({ 100.0f, winSize.y - 100.0f });
	hpFrameS_->SetSize({ 440.0f, 40.0f });

	hpFrameShadowS_ = std::make_unique<Sprite>();
	hpFrameShadowS_->SetAnchorPoint({ 0.0f, 0.0f });
	hpFrameShadowS_->SetPosition({ 100.0f, winSize.y - 100.0f });
	hpFrameShadowS_->SetSize({ 440.0f, 40.0f });
#pragma endregion

#pragma region 画像ハンドル
	hpBarH_ = LoadTexture("Sprite/hpBar.png");
	hpFrameH_ = LoadTexture("Sprite/hpFrame.png");
	hpFrameShadowH_ = LoadTexture("Sprite/hpFrameShadow.png");
#pragma endregion
}

void PlayerUI::Update()
{

}

void PlayerUI::Draw()
{
	hpFrameShadowS_->Draw(hpFrameShadowH_);
	hpBarS_->Draw(hpBarH_);
	hpFrameS_->Draw(hpFrameH_);
}

void PlayerUI::OnCollision()
{

}

void PlayerUI::MatUpdate()
{
	hpBarS_->MatUpdate();
	hpFrameS_->MatUpdate();
	hpFrameShadowS_->MatUpdate();
}