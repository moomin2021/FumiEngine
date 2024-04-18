#include "AudioLayer.h"
#include "Texture.h"

void AudioLayer::Initialize()
{
#pragma region ボタン
	buttons_.resize(2);
	BoxButton::SetCollisionManager2D(CollisionManager2D::GetInstance());
	for (uint16_t i = 0; i < buttons_.size(); i++)
	{
		buttons_[i] = std::make_unique<BoxButton>();
		buttons_[i]->Initialize(NO_TAG, { 544.0f, 220.0f + (i * 70.0f) }, { 746.0f, 60.0f }, { 758.0f, 72.0f },
			LoadTexture("Sprite/leafButton.png"),
			LoadTexture("Sprite/leafButtonText1_" + std::string(std::to_string(i)) + std::string(".png")),
			LoadTexture("Sprite/leafButtonFrame.png"));
	}
#pragma endregion

#pragma region スライダー
	sliders_.resize(2);
	Slider::SetCollisionManager2D(CollisionManager2D::GetInstance());
	for (uint8_t i = 0; i < sliders_.size(); i++)
	{
		sliders_[i] = std::make_unique<Slider>();
		sliders_[i]->Initialize({ 700.0f, 220.0f + (i * 70.0f) }, { 288.0f, 13.0f }, { 39.0f, 39.0f },
			LoadTexture("Sprite/slider.png"),
			LoadTexture("Sprite/sliderPoint.png"));
	}
#pragma endregion
}

void AudioLayer::Update()
{
	for (auto& it : buttons_) it->Update();
	for (auto& it : sliders_) it->Update();
	for (auto& it : buttons_) it->SetIsCollision(isValid_);
	for (auto& it : sliders_) it->SetIsCollision(isValid_);
}

void AudioLayer::Draw()
{
	if (isValid_ == false) return;
	for (auto& it : buttons_) it->Draw();
	for (auto& it : sliders_) it->Draw();
}

void AudioLayer::Collision()
{
	for (auto& it : buttons_) it->OnCollision();
	for (auto& it : sliders_) it->OnCollision();
}

void AudioLayer::MatUpdate()
{
	for (auto& it : buttons_) it->MatUpdate();
	for (auto& it : sliders_) it->MatUpdate();
}

void AudioLayer::Finalize()
{

}