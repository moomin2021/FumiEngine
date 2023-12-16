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

	reloadUIS_ = std::make_unique<Sprite>();
	reloadUIS_->SetAnchorPoint({ 0.5f, 0.5f });
	reloadUIS_->SetPosition({ winSize.x / 2.0f, winSize.y / 2.0f });
	reloadUIS_->SetSize({ 100.0f, 100.0f });

	crossHairS_ = std::make_unique<Sprite>();
	crossHairS_->SetAnchorPoint({ 0.5f, 0.5f });
	crossHairS_->SetPosition({ winSize.x / 2.0f, winSize.y / 2.0f });
	crossHairS_->SetSize({ 26.0f, 26.0f });
#pragma endregion

#pragma region 画像ハンドル
	hpBarH_ = LoadTexture("Sprite/hpBar.png");
	hpFrameH_ = LoadTexture("Sprite/hpFrame.png");
	hpFrameShadowH_ = LoadTexture("Sprite/hpFrameShadow.png");
	reloadUIH_ = LoadTexture("Sprite/reloadUI.png");
	crossHairH_ = LoadTexture("Sprite/crossHair.png");
#pragma endregion
}

void PlayerUI::Update()
{
	float rate = (float)pPlayer_->GetHP() / pPlayer_->GetMAXHP();
	float result = hpBarSize_.x * rate;
	hpBarS_->SetUV({ rate, 1.0f });
	hpBarS_->SetSize({ result, hpBarSize_.y });

	// リロードUI画像の角度
	static float rotaY = 0.0f;

	if (pPlayer_->GetIsReload())
	{
		rotaY -= 3.0f;
		reloadUIS_->SetRotation(rotaY);
	}
}

void PlayerUI::Draw()
{
	hpFrameShadowS_->Draw(hpFrameShadowH_);
	hpBarS_->Draw(hpBarH_);
	hpFrameS_->Draw(hpFrameH_);
	if (pPlayer_->GetIsReload())reloadUIS_->Draw(reloadUIH_);
	if (!pPlayer_->GetIsReload()) crossHairS_->Draw(crossHairH_);
}

void PlayerUI::OnCollision()
{

}

void PlayerUI::MatUpdate()
{
	hpBarS_->MatUpdate();
	hpFrameS_->MatUpdate();
	hpFrameShadowS_->MatUpdate();
	reloadUIS_->MatUpdate();
	crossHairS_->MatUpdate();
}