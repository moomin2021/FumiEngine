#include "HitFrame.h"
#include "Util.h"
#include "Easing.h"

#include "CollisionAttribute.h"
#include "SceneManager.h"

HitFrame::~HitFrame()
{
	colMgr2D_->RemoveCollider(cMouse_.get());
}

void HitFrame::Initialize()
{
#pragma region インスタンス
	mouse_ = Mouse::GetInstance();
	colMgr2D_ = CollisionManager2D::GetInstance();
#pragma endregion

#pragma region スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region マウスのコライダー
	cMouse_ = std::make_unique<PointCollider>();
	cMouse_->SetAttribute(COL_POINT);
	colMgr2D_->AddCollider(cMouse_.get());
#pragma endregion
}

void HitFrame::Update()
{
	// マウスの座標とコライダーの座標を合わせる
	cMouse_->SetOffset(mouse_->MousePos());

	// フレームのイージング処理
	FrameEasing();
}

void HitFrame::Draw()
{
	// 描画フラグが[ON]なら描画
	if (isDraw_) sprite_->Draw(handle_);
}

void HitFrame::OnCollision(SelectNum& selectNum)
{
	// マウスが衝突していたら以下の処理をする
	// 1. 衝突フラグを[ON]にする
	// 2. 保存している属性と衝突している属性が違ったらイージング開始時間を記録
	// 3. ボタンの属性を衝突相手のコライダーから取得
	// 4. 衝突相手からスプライトの座標を読み取り設定
	if (cMouse_->GetIsHit())
	{
		isDraw_ = true;
		if (AttrBeforeAndNowDiffer()) startEaseTime_ = Util::GetTimrMSec();
		buttonAttr_ = (SelectNum)cMouse_->GetHitCollider()->GetTag();
		sprite_->SetPosition(cMouse_->GetHitCollider()->GetSpritePos());
	}

	// マウスが衝突していなかったら以下を処理する
	// 1. 衝突フラグを[OFF]にする
	// 2. ボタンの属性をなしにする
	else
	{
		isDraw_ = false;
		selectNum = SelectNum::NONE;
		buttonAttr_ = SelectNum::NONE;
	}

	// 左クリックでアクション
	if (mouse_->TriggerMouseButton(M_LEFT))
	{
		selectNum = buttonAttr_;
	}
}

void HitFrame::MatUpdate()
{
	sprite_->MatUpdate();
}

void HitFrame::FrameEasing()
{
	float elapsedTime = (Util::GetTimrMSec() - startEaseTime_) / 1000.0f;
	float rate = elapsedTime / easeTime_;
	rate = Util::Clamp(rate, 1.0f, 0.0f);
	Vector2 resultSize = { 0.0f, 0.0f };
	resultSize.x = Easing::Quint::easeOut(size_.x * maxSizeRate_, size_.x, rate);
	resultSize.y = Easing::Quint::easeOut(size_.y * maxSizeRate_, size_.y, rate);
	sprite_->SetSize(resultSize);
}

void HitFrame::SetFrame(const Vector2 inSize, uint16_t inHandle)
{
	// サイズを保存
	size_ = inSize;

	// 画像ハンドルを保存
	handle_ = inHandle;
}

void HitFrame::SetIsCollision(bool frag)
{
	cMouse_->SetIsOnCol(frag);
}

bool HitFrame::AttrBeforeAndNowDiffer()
{
	return buttonAttr_ != (SelectNum)cMouse_->GetHitCollider()->GetTag();
}