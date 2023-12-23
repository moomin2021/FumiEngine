#include "Number.h"

#include "Texture.h"

void Number::Initialize(uint16_t digit, const Vector2& inPos, const Vector2& inSize, const std::string numFileName)
{
#pragma region スプライト
	numS_.resize(digit);

	for (size_t i = 0; i < digit; i++)
	{
		numS_[i] = std::make_unique<Sprite>();
		numS_[i]->SetAnchorPoint({ 0.5f, 0.5f });
		numS_[i]->SetPosition({ inPos.x - (i * inSize.x), inPos.y });
		numS_[i]->SetSize(inSize);
	}
#pragma endregion

#pragma region 画像
	handles_.resize(10);
	for (size_t i = 0; i < 10; i++)
	{
		handles_[i] = LoadTexture(numFileName + std::string("/") + std::string(std::to_string(i)) + std::string(".png"));
	}
#pragma endregion
}

void Number::MatUpdate()
{
	for (auto& it : numS_) it->MatUpdate();
}

void Number::Draw(uint16_t inValue)
{
	uint16_t value = inValue;

	for (size_t i = 0; i < numS_.size(); i++)
	{
		numS_[i]->Draw(handles_[value % 10]);
		value /= 10;
	}
}