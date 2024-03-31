#include "PlayerUI.h"
#include "Texture.h"
#include "WinAPI.h"

PlayerUI::PlayerUI(Player* inPlayer, Weapon* inWeapon) : pPlayer_(inPlayer), pWeapon_(inWeapon) {}

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
	hpBarS_->SetPosition({ 72.0f, 60.0f });
	hpBarS_->SetSize({ 434.0f, 34.0f });

	hpFrameS_ = std::make_unique<Sprite>();
	hpFrameS_->SetAnchorPoint({ 0.0f, 0.0f });
	hpFrameS_->SetPosition({ 70.0f, 60.0f });
	hpFrameS_->SetSize({ 440.0f, 40.0f });

	hpFrameShadowS_ = std::make_unique<Sprite>();
	hpFrameShadowS_->SetAnchorPoint({ 0.0f, 0.0f });
	hpFrameShadowS_->SetPosition({ 70.0f, 60.0f });
	hpFrameShadowS_->SetSize({ 440.0f, 40.0f });

	reloadUIS_ = std::make_unique<Sprite>();
	reloadUIS_->SetAnchorPoint({ 0.5f, 0.5f });
	reloadUIS_->SetPosition({ winSize.x / 2.0f, winSize.y / 2.0f });
	reloadUIS_->SetSize({ 100.0f, 100.0f });

	reloadBackUIS_ = std::make_unique<Sprite>();
	reloadBackUIS_->SetAnchorPoint({ 0.5f, 0.5f });
	reloadBackUIS_->SetPosition({ winSize.x / 2.0f + 5.0f, winSize.y / 2.0f + 10.0f });
	reloadBackUIS_->SetSize({ 100.0f, 100.0f });

	crossHairS_ = std::make_unique<Sprite>();
	crossHairS_->SetAnchorPoint({ 0.5f, 0.5f });
	crossHairS_->SetPosition({ winSize.x / 2.0f, winSize.y / 2.0f });
	crossHairS_->SetSize({ 26.0f, 26.0f });

	// 残弾数表示UI用のスプライトを生成
	sBulletValueDisplayFrame_ = std::make_unique<Sprite>();
	sBulletValueDisplayFrame_->SetAnchorPoint({ 0.5f, 0.5f });
	sBulletValueDisplayFrame_->SetSize({ 300.0f, 140.0f });
	sBulletValueDisplayFrame_->SetPosition({ winSize.x - 150.0f, winSize.y - 120.0f });

	// 最大弾数表示用スプライト生成
	sMaxBulletUI_.resize(2);
	sMaxBulletUI_[0] = std::make_unique<Sprite>();
	sMaxBulletUI_[1] = std::make_unique<Sprite>();
	sMaxBulletUI_[0]->SetAnchorPoint({ 1.0f, 1.0f });
	sMaxBulletUI_[1]->SetAnchorPoint({ 1.0f, 1.0f });
	sMaxBulletUI_[0]->SetSize({ 35.25f, 54.0f });
	sMaxBulletUI_[1]->SetSize({ 35.25f, 54.0f });
	sMaxBulletUI_[0]->SetPosition({ winSize.x - 85.75f, winSize.y - 80.0f });
	sMaxBulletUI_[1]->SetPosition({ winSize.x - 45.0f, winSize.y - 80.0f });

	// 残弾数表示スプライト
	sNowBulletUI_.resize(2);
	sNowBulletUI_[0] = std::make_unique<Sprite>();
	sNowBulletUI_[1] = std::make_unique<Sprite>();
	sNowBulletUI_[0]->SetAnchorPoint({ 0.5f, 1.0f });
	sNowBulletUI_[1]->SetAnchorPoint({ 0.5f, 1.0f });
	sNowBulletUI_[0]->SetSize({ 47.0f, 72.0f });
	sNowBulletUI_[1]->SetSize({ 47.0f, 72.0f });
	sNowBulletUI_[0]->SetPosition({ winSize.x - 247.0f, winSize.y - 80.0f });
	sNowBulletUI_[1]->SetPosition({ winSize.x - 195.0f, winSize.y - 80.0f });
#pragma endregion

#pragma region 画像ハンドル
	hpBarH_ = LoadTexture("Sprite/hpBar.png");
	hpFrameH_ = LoadTexture("Sprite/hpFrame.png");
	hpFrameShadowH_ = LoadTexture("Sprite/hpFrameShadow.png");
	reloadUIH_ = LoadTexture("Sprite/reloadUI.png");
	reloadBackUIH_ = LoadTexture("Sprite/reloadBackUI.png");
	crossHairH_ = LoadTexture("Sprite/crossHair.png");
	bulletValueDisplayFrameHandle_ = LoadTexture("Sprite/BulletValueDisplayFrame.png");
	numberHandle_.resize(10);
	numberHandle_[0] = LoadTexture("Sprite/number0.png");
	numberHandle_[1] = LoadTexture("Sprite/number1.png");
	numberHandle_[2] = LoadTexture("Sprite/number2.png");
	numberHandle_[3] = LoadTexture("Sprite/number3.png");
	numberHandle_[4] = LoadTexture("Sprite/number4.png");
	numberHandle_[5] = LoadTexture("Sprite/number5.png");
	numberHandle_[6] = LoadTexture("Sprite/number6.png");
	numberHandle_[7] = LoadTexture("Sprite/number7.png");
	numberHandle_[8] = LoadTexture("Sprite/number8.png");
	numberHandle_[9] = LoadTexture("Sprite/number9.png");
#pragma endregion
}

void PlayerUI::Update()
{
	HPUI();

	// リロードUI画像の角度
	static float rotaY = 0.0f;

	if (pWeapon_->GetIsReload())
	{
		rotaY -= 3.0f;
		reloadUIS_->SetRotation(rotaY);
		reloadBackUIS_->SetRotation(rotaY);
	}
}

void PlayerUI::Draw()
{
	// 最大弾数を表示
	sMaxBulletUI_[0]->Draw(numberHandle_[pWeapon_->GetMaxBullet() / 10]);
	sMaxBulletUI_[1]->Draw(numberHandle_[pWeapon_->GetMaxBullet() % 10]);

	// 残弾数を表示
	sNowBulletUI_[0]->Draw(numberHandle_[pWeapon_->GetNowBullet() / 10]);
	sNowBulletUI_[1]->Draw(numberHandle_[pWeapon_->GetNowBullet() % 10]);

	// 残弾数表示枠を描画
	sBulletValueDisplayFrame_->Draw(bulletValueDisplayFrameHandle_);

	hpFrameShadowS_->Draw(hpFrameShadowH_);
	hpBarS_->Draw(hpBarH_);
	hpFrameS_->Draw(hpFrameH_);
	if (pWeapon_->GetIsReload())reloadBackUIS_->Draw(reloadBackUIH_);
	if (pWeapon_->GetIsReload())reloadUIS_->Draw(reloadUIH_);
	if (!pWeapon_->GetIsReload()) crossHairS_->Draw(crossHairH_);
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
	reloadBackUIS_->MatUpdate();
	crossHairS_->MatUpdate();
	// 残弾数表示UIハンドル
	sBulletValueDisplayFrame_->MatUpdate();

	// 最大弾数表示スプライト
	sMaxBulletUI_[0]->MatUpdate();
	sMaxBulletUI_[1]->MatUpdate();

	// 残弾数表示スプライト
	sNowBulletUI_[0]->MatUpdate();
	sNowBulletUI_[1]->MatUpdate();
}

void PlayerUI::HPUI()
{
	float rate = (float)pPlayer_->GetNowHP() / pPlayer_->GetMaxHP();
	float result = hpBarSize_.x * rate;
	hpBarS_->SetUV({ rate, 1.0f });
	hpBarS_->SetSize({ result, hpBarSize_.y });
}