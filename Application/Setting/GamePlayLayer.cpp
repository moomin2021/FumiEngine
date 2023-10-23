#include "GamePlayLayer.h"

GamePlayLayer::~GamePlayLayer()
{
}

void GamePlayLayer::Initialize()
{
#pragma region インスタンス
	mouse_ = Mouse::GetInstance();
	colMgr2D_ = CollisionManager2D::GetInstance();
#pragma endregion

#pragma region スライダー
	sSlider_ = std::make_unique<Sprite>();
	sSlider_->SetAnchorPoint({ 0.5f, 0.5f });
	sSlider_->SetPosition({});
	sSlider_->SetSize({288.0f, 13.0f});

	sSliderPoint_ = std::make_unique<Sprite>();
	sSliderPoint_->SetAnchorPoint({ 0.5f, 0.5f });
	sSliderPoint_->SetPosition({});
	sSliderPoint_->SetSize({ 39.0f, 39.0f });


#pragma endregion
}

void GamePlayLayer::Update()
{
}

void GamePlayLayer::Draw()
{
}

void GamePlayLayer::OnCollision()
{
}

void GamePlayLayer::MatUpdate()
{
}
//
//void GamePlayLayer::SetIsCollision(bool frag)
//{
//}