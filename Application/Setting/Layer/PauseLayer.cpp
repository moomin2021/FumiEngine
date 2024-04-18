#include "PauseLayer.h"
#include "Texture.h"

void PauseLayer::Initialize()
{
	// ボタンの生成＆設定
	buttons_.resize(3);
	for (uint16_t i = 0; i < buttons_.size(); i++)
	{
		buttons_[i] = std::make_unique<BoxButton>();
		buttons_[i]->Initialize(i, { 1920.0f / 2.0f, (1080.0f / 2.0f) + (i * 50.0f) + -50.0f }, { 260.0f, 44.0f }, { 272.0f, 56.0f },
			LoadTexture("Sprite/treeButton.png"),
			LoadTexture("Sprite/pauseText" + std::string(std::to_string(i)) + std::string(".png")),
			LoadTexture("Sprite/treeButtonCursorFrame.png"));
	}
}

void PauseLayer::Update()
{
	for (auto& it : buttons_) it->Update();
	for (auto& it : buttons_) it->SetIsCollision(isValid_);
}

void PauseLayer::Draw()
{
	if (isValid_ == false) return;
	for (auto& it : buttons_) it->Draw();
}

void PauseLayer::Collision()
{
	for (auto& it : buttons_) it->OnCollision();
}

void PauseLayer::MatUpdate()
{
	for (auto& it : buttons_) it->MatUpdate();
}

void PauseLayer::Finalize()
{

}