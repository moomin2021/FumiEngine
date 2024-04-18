#include "GamePlayLayer.h"
#include "Texture.h"

#include "GameSettingData.h"

void GamePlayLayer::Initialize()
{
#pragma region ボタン
	buttons_.resize(1);
	BoxButton::SetCollisionManager2D(CollisionManager2D::GetInstance());
	for (uint16_t i = 0; i < buttons_.size(); i++)
	{
		buttons_[i] = std::make_unique<BoxButton>();
		buttons_[i]->Initialize(NO_TAG, { 544.0f, 220.0f + (i * 70.0f) }, { 746.0f, 60.0f }, { 758.0f, 72.0f },
			LoadTexture("Sprite/leafButton.png"),
			LoadTexture("Sprite/leafButtonText0_" + std::string(std::to_string(i)) + std::string(".png")),
			LoadTexture("Sprite/leafButtonFrame.png"));
	}
#pragma endregion

#pragma region スライダー
	GameSettingData* data = GameSettingData::GetInstance();
	sliders_.resize(1);
	Slider::SetCollisionManager2D(CollisionManager2D::GetInstance());
	for (uint8_t i = 0; i < sliders_.size(); i++)
	{
		sliders_[i] = std::make_unique<Slider>();
		sliders_[i]->SetValue(data->sensitivity_);
		sliders_[i]->Initialize({ 700.0f, 220.0f + (i * 70.0f) }, { 288.0f, 13.0f }, { 39.0f, 39.0f },
			LoadTexture("Sprite/slider.png"),
			LoadTexture("Sprite/sliderPoint.png"));
	}
#pragma endregion
}

void GamePlayLayer::Update()
{
	for (auto& it : buttons_) it->Update();
	for (auto& it : sliders_) it->Update();
	for (auto& it : buttons_) it->SetIsCollision(isValid_);
	for (auto& it : sliders_) it->SetIsCollision(isValid_);

	GameSettingData* data = GameSettingData::GetInstance();
	data->sensitivity_ = sliders_[0]->GetValue();
}

void GamePlayLayer::Draw()
{
	if (isValid_ == false) return;
	for (auto& it : buttons_) it->Draw();
	for (auto& it : sliders_) it->Draw();
}

void GamePlayLayer::Collision()
{
	for (auto& it : buttons_) it->OnCollision();
	for (auto& it : sliders_) it->OnCollision();
}

void GamePlayLayer::MatUpdate()
{
	for (auto& it : buttons_) it->MatUpdate();
	for (auto& it : sliders_) it->MatUpdate();
}

void GamePlayLayer::Finalize()
{

}