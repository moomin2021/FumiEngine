#include "ResultBox.h"
#include "CollisionAttribute.h"

CollisionManager2D* ResultBox::sColMgr2D_ = nullptr;

ResultBox::~ResultBox()
{
	sColMgr2D_->RemoveCollider(boxC_.get());
}

void ResultBox::Initialize(const Vector2& inPos, const Vector2& inSize, uint16_t inBoxHandle, uint16_t inTextHandle)
{
#pragma region スプライト
	boxS_ = std::make_unique<Sprite>();
	boxS_->SetAnchorPoint({ 0.5f, 0.5f });
	boxS_->SetPosition(inPos);
	boxS_->SetSize(inSize);

	textS_ = std::make_unique<Sprite>();
	textS_->SetAnchorPoint({ 0.5f, 0.5f });
	textS_->SetPosition(inPos);
	textS_->SetSize(inSize);
#pragma endregion

#pragma region 画像
	boxH_ = inBoxHandle;
	textH_ = inTextHandle;
#pragma endregion

#pragma region コライダー
	boxC_ = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, inSize / 2.0f);
	boxC_->SetAttribute(COL_BOX);
	boxC_->SetSprite(boxS_.get());
	sColMgr2D_->AddCollider(boxC_.get());
#pragma endregion
}

void ResultBox::Update()
{
}

void ResultBox::Draw()
{
	boxS_->Draw(boxH_);
	textS_->Draw(textH_);
}

void ResultBox::OnCollision()
{
	boxS_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	if (boxC_->GetIsHit())
	{
		boxS_->SetColor({1.0f, 1.0f, 0.0f, 1.0f});
	}
}

void ResultBox::MatUpdate()
{
	boxS_->MatUpdate();
	textS_->MatUpdate();
}