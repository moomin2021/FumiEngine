#include "SettingLayer.h"
#include "Texture.h"
#include "CollisionAttribute.h"
#include "Util.h"
#include "Easing.h"

SettingLayer::~SettingLayer()
{
	colMgr2D_->RemoveCollider(mouseCol_.get());
}

void SettingLayer::Initialize()
{
#pragma region インスタンス
	mouse_ = Mouse::GetInstance();
	colMgr2D_ = CollisionManager2D::GetInstance();
#pragma endregion

#pragma region スプライト
	sTreeButtons_.resize(2);
	for (uint8_t i = 0; i < sTreeButtons_.size(); i++)
	{
		sTreeButtons_[i] = std::make_unique<Sprite>();
		sTreeButtons_[i]->SetPosition({ 300.0f + (i * 280.0f), 101.0f });
		sTreeButtons_[i]->SetSize({ 260.0f, 44.0f });
		sTreeButtons_[i]->SetAnchorPoint({ 0.5f, 0.5f });
	}

	sTreeButtonCursorFrame_ = std::make_unique<Sprite>();
	sTreeButtonCursorFrame_->SetPosition({ 300.0f, 101.0f });
	sTreeButtonCursorFrame_->SetSize({ 272.0f, 56.0f });
	sTreeButtonCursorFrame_->SetAnchorPoint({ 0.5f, 0.5f });

	sBackBox_ = std::make_unique<Sprite>();
	sBackBox_->SetPosition({ 160.0f, 176.0f });
	sBackBox_->SetSize({ 768.0f, 746.0f });
#pragma endregion

#pragma region 画像
	gTreeButton_ = LoadTexture("Resources/treeButton.png");
	gTreeButtonCursorFrame_ = LoadTexture("Resources/treeButtonCursorFrame.png");
	gBackBox_ = LoadTexture("Resources/backBox.png");
#pragma endregion

#pragma region コライダー
	mouseCol_ = std::make_unique<PointCollider>();
	mouseCol_->SetAttribute(COL_POINT);
	colMgr2D_->AddCollider(mouseCol_.get());

	treeButtonsCol_.resize(2);
	for (uint8_t i = 0; i < treeButtonsCol_.size(); i++)
	{
		treeButtonsCol_[i] = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, Vector2{ 130.0f, 22.0f });
		treeButtonsCol_[i]->SetAttribute(COL_BOX);
		treeButtonsCol_[i]->SetSprite(sTreeButtons_[i].get());
		colMgr2D_->AddCollider(treeButtonsCol_[i].get());
	}
#pragma endregion

#pragma region ボタン関連
	treeButtonPos_.resize(2);
	for (uint8_t i = 0; i < treeButtonPos_.size(); i++)
	{
		treeButtonPos_[i] = { 300.0f + (i * 280.0f), 101.0f };
	}
#pragma endregion
}

void SettingLayer::Update()
{
	// マウスの衝突判定の更新
	mouseCol_->SetOffset(mouse_->MousePos());

	// ボタンのフレームの処理
	ButtonFrame();
}

void SettingLayer::Draw()
{
	// 背景のボックス
	sBackBox_->Draw(gBackBox_);

	// ツリーボタン
	for (auto& it : sTreeButtons_) it->Draw(gTreeButton_);

	// カーソルがあった時のフレーム
	if (isTreeButton_) sTreeButtonCursorFrame_->Draw(gTreeButtonCursorFrame_);
}

void SettingLayer::OnCollision()
{
	bool result = false;
	for (uint16_t i = 0; i < treeButtonsCol_.size(); i++)
	{
		sTreeButtons_[i]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		if (treeButtonsCol_[i]->GetIsHit())
		{
			if (isTreeButton_ == false) startEaseTime_ = Util::GetTimrMSec();// イージング開始時間を記録
			if ((uint16_t)cursorState_ != i) startEaseTime_ = Util::GetTimrMSec();// イージング開始時間を記録
			result = true;// 選択中フラグを[ON]
			sTreeButtons_[i]->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });// 色を変える
			cursorState_ = (SelectState)i;// 選択しているものを保存
			sTreeButtonCursorFrame_->SetPosition(treeButtonPos_[i]);
		}
	}

	isTreeButton_ = result;
	if (isTreeButton_ == false) cursorState_ = SelectState::NONE;
}

void SettingLayer::MatUpdate()
{
	// ツリーボタン
	for (auto& it : sTreeButtons_) it->MatUpdate();

	// 背景のボックス
	sBackBox_->MatUpdate();

	// カーソルがあった時のフレーム
	sTreeButtonCursorFrame_->MatUpdate();
}

void SettingLayer::ButtonFrame()
{
	if (isTreeButton_)
	{
		float elapsedTime = (Util::GetTimrMSec() - startEaseTime_) / 1000.0f;
		float rate = elapsedTime / easeTime_;
		rate = Util::Clamp(rate, 1.0f, 0.0f);
		Vector2 resultPos = { 0.0f, 0.0f };
		resultPos.x = Easing::Quint::easeOut(startTreeButtonFrameSize_.x, endTreeButtonFrameSize_.x, rate);
		resultPos.y = Easing::Quint::easeOut(startTreeButtonFrameSize_.y, endTreeButtonFrameSize_.y, rate);
		sTreeButtonCursorFrame_->SetSize(resultPos);
	}
}