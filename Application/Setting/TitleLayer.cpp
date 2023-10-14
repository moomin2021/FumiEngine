#include "TitleLayer.h"
#include "CollisionManager2D.h"
#include "Texture.h"

TitleLayer::~TitleLayer()
{
}

void TitleLayer::Initialize()
{
	buttons_.resize(3);
	BoxButton::SetCollisionManager2D(CollisionManager2D::GetInstance());
	for (uint16_t i = 0; i < buttons_.size(); i++)
	{
		buttons_[i] = std::make_unique<BoxButton>();
		buttons_[i]->Initialize(i, { 250.0f, 525.0f + (i * 50.0f) }, { 310.0f, 40.0f },
			LoadTexture("Resources/titleSelectButton.png"),
			LoadTexture("resources/titleSelectText" + std::string(std::to_string(i)) + std::string(".png")));
	}
}

void TitleLayer::Update()
{
	for (auto& it : buttons_) it->Update();
}

void TitleLayer::Draw()
{
	for (auto& it : buttons_) it->Draw();
}

void TitleLayer::OnCollision()
{
	for (auto& it : buttons_) it->OnCollision();
}

void TitleLayer::MatUpdate()
{
	for (auto& it : buttons_) it->MatUpdate();
}