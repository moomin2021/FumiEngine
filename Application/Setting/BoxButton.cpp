#include "BoxButton.h"
#include "CollisionAttribute.h"

CollisionManager2D* BoxButton::stColMgr2D_ = nullptr;

BoxButton::~BoxButton()
{
	stColMgr2D_->RemoveCollider(cButton_.get());
}

void BoxButton::Initialize(int32_t inTag, const Vector2& inPos, const Vector2& inSize, uint16_t inButtonHandle, uint16_t inTextHandle)
{
#pragma region メンバ変数
	sButton_ = std::make_unique<Sprite>();
	sButton_->SetAnchorPoint({ 0.5f, 0.5f });
	sButton_->SetPosition(inPos);
	sButton_->SetSize(inSize);

	sText_ = std::make_unique<Sprite>();
	sText_->SetAnchorPoint({ 0.5f, 0.5f });
	sText_->SetPosition(inPos);
	sText_->SetSize(inSize);
#pragma endregion

#pragma region 画像
	hButton_ = inButtonHandle;
	hText_ = inTextHandle;
#pragma endregion

#pragma region コライダー
	cButton_ = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, inSize / 2.0f);
	cButton_->SetAttribute(COL_BOX);
	cButton_->SetSprite(sButton_.get());
	cButton_->SetTag(inTag);
	stColMgr2D_->AddCollider(cButton_.get());
#pragma endregion
}

void BoxButton::Update()
{
}

void BoxButton::Draw()
{
	// ボタン
	sButton_->Draw(hButton_);

	// テキスト
	sText_->Draw(hText_);
}

void BoxButton::OnCollision()
{
	// 当たっていたらボタンの色を変える
	if (cButton_->GetIsHit())
	{
		sButton_->SetColor({1.0f, 1.0f, 0.0f, 1.0f});
	}

	else
	{
		sButton_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void BoxButton::MatUpdate()
{
	// ボタン
	sButton_->MatUpdate();

	// テキスト
	sText_->MatUpdate();
}

void BoxButton::SetIsCollision(bool frag)
{
	cButton_->SetIsOnCol(frag);
}