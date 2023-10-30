#include "Slider.h"

#include "CollisionAttribute.h"
#include "Easing.h"
#include "Util.h"

CollisionManager2D* Slider::stColMgr2D_ = nullptr;

Slider::~Slider()
{
	stColMgr2D_->RemoveCollider(cThumb_.get());
}

void Slider::Initialize(const Vector2& inTrackPos, const Vector2& inTrackSize, const Vector2& inThumbSize, uint16_t inTrackHandle, uint16_t inThumbHandle)
{
#pragma region インスタンス
	mouse_ = Mouse::GetInstance();
#pragma endregion

#pragma region スプライト
	sTrack_ = std::make_unique<Sprite>();
	sTrack_->SetAnchorPoint({ 0.5f, 0.5f });
	sTrack_->SetPosition(inTrackPos);
	sTrack_->SetSize(inTrackSize);

	sThumb_ = std::make_unique<Sprite>();
	sThumb_->SetAnchorPoint({ 0.5f, 0.5f });
	sThumb_->SetPosition(inTrackPos);
	sThumb_->SetSize(inThumbSize);
#pragma endregion

#pragma region 画像
	gTrack_ = inTrackHandle;
	gThumb_ = inThumbHandle;
#pragma endregion

#pragma region コライダー
	cThumb_ = std::make_unique<BoxCollider>(Vector2(0.0f, 0.0f), inThumbSize / 2.0f);
	cThumb_->SetAttribute(COL_SLIDER);
	cThumb_->SetSprite(sThumb_.get());
	stColMgr2D_->AddCollider(cThumb_.get());
#pragma endregion

#pragma region Thumbの最小の位置と最大の位置
	minPos_ = { inTrackPos.x - inTrackSize.x / 2.0f, inTrackPos.y };
	maxPos_ = { inTrackPos.x + inTrackSize.x / 2.0f, inTrackPos.y };
#pragma endregion
}

void Slider::Update()
{
}

void Slider::Draw()
{
	sTrack_->Draw(gTrack_);
	sThumb_->Draw(gThumb_);
}

void Slider::OnCollision()
{
	if (cThumb_->GetIsHit() && mouse_->TriggerMouseButton(M_LEFT))
	{
		isHold_ = true;
	}

	if (mouse_->PushMouseButton(M_LEFT) == false)
	{
		isHold_ = false;
	}

	if (isHold_)
	{
		Vector2 result = { 0.0f, maxPos_.y };
		result.x = Util::Clamp(mouse_->MousePos().x, maxPos_.x, minPos_.x);
		sThumb_->SetPosition(result);
	}
}

void Slider::MatUpdate()
{
	sTrack_->MatUpdate();
	sThumb_->MatUpdate();
}

void Slider::SetIsCollision(bool frag)
{
	cThumb_->SetIsOnCol(frag);
}