#include "BoxButton.h"

#include "Util.h"
#include "Easing.h"
#include "CollisionAttribute.h"

CollisionManager2D* BoxButton::stColMgr2D_ = nullptr;

BoxButton::~BoxButton()
{
	stColMgr2D_->RemoveCollider(cButton_.get());
}

void BoxButton::Initialize(int32_t inTag, const Vector2& inPos, const Vector2& inButtonSize, const Vector2& inFrameSize, uint16_t inButtonHandle, uint16_t inTextHandle, uint16_t inFrameHandle)
{
#pragma region スプライト
	sButton_ = std::make_unique<Sprite>();
	sButton_->SetAnchorPoint({ 0.5f, 0.5f });
	sButton_->SetPosition(inPos);
	sButton_->SetSize(inButtonSize);

	sText_ = std::make_unique<Sprite>();
	sText_->SetAnchorPoint({ 0.5f, 0.5f });
	sText_->SetPosition(inPos);
	sText_->SetSize(inButtonSize);

	sFrame_ = std::make_unique<Sprite>();
	sFrame_->SetAnchorPoint({ 0.5f, 0.5f });
	sFrame_->SetPosition(inPos);
	sFrame_->SetSize(inFrameSize);
	size_ = inFrameSize;
#pragma endregion

#pragma region 画像
	gButton_ = inButtonHandle;
	gText_ = inTextHandle;
	gFrame_ = inFrameHandle;
#pragma endregion

#pragma region コライダー
	cButton_ = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, inButtonSize / 2.0f);
	cButton_->SetAttribute(COL_BOX);
	cButton_->SetSprite(sButton_.get());
	cButton_->SetTag(inTag);
	stColMgr2D_->AddCollider(cButton_.get());
#pragma endregion
}

void BoxButton::Update()
{
	// フレームイージング
	FrameEasing();
}

void BoxButton::Draw()
{
	// ボタン
	sButton_->Draw(gButton_);

	// テキスト
	sText_->Draw(gText_);

	// フレーム
	if (isDraw_) sFrame_->Draw(gFrame_);
}

void BoxButton::OnCollision()
{
	// ボタンが衝突していたら以下の処理をする
	if (cButton_->GetIsHit())
	{
		if (isDraw_ == false) startEaseTime_ = Util::GetTimrMSec();
		isDraw_ = true;
		sButton_->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	}

	// ボタンが衝突していなかったら以下の処理をする
	else
	{
		isDraw_ = false;
		sButton_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void BoxButton::MatUpdate()
{
	// ボタン
	sButton_->MatUpdate();

	// テキスト
	sText_->MatUpdate();

	// フレーム
	sFrame_->MatUpdate();
}

void BoxButton::FrameEasing()
{
	float elapsedTime = (Util::GetTimrMSec() - startEaseTime_) / 1000.0f;
	float rate = elapsedTime / easeTime_;
	rate = Util::Clamp(rate, 1.0f, 0.0f);
	Vector2 resultSize = { 0.0f, 0.0f };
	resultSize.x = Easing::Quint::easeOut(size_.x * maxSizeRate_, size_.x, rate);
	resultSize.y = Easing::Quint::easeOut(size_.y * maxSizeRate_, size_.y, rate);
	sFrame_->SetSize(resultSize);
}

void BoxButton::SetIsCollision(bool frag)
{
	cButton_->SetIsOnCol(frag);
}