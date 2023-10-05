#include "TitleScene.h"
#include "Texture.h"
#include "PipelineManager.h"
#include "SceneManager.h"
#include "CollisionAttribute.h"

#include <imgui_impl_DX12.h>

TitleScene::TitleScene() {}

TitleScene::~TitleScene()
{
	colMgr2D_->RemoveCollider(mouseCol_.get());
	for (auto& it : selectButtonsCol_) colMgr2D_->RemoveCollider(it.get());
}

void TitleScene::Initialize()
{
#pragma region インスタンス取得
	key_ = Key::GetInstance();// キーボード
	mouse_ = Mouse::GetInstance();// マウス
	colMgr2D_ = CollisionManager2D::GetInstance();// 衝突マネージャー2D
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	Sprite::SetCamera(camera_.get());
#pragma endregion

#pragma region スプライト
	sTitle_ = std::make_unique<Sprite>();
	sTitle_->SetSize(Vector2{ 1920.0f, 1080.0f });

	sSelectButtons_.resize(2);
	for (uint16_t i = 0; i < sSelectButtons_.size(); i++) {
		sSelectButtons_[i] = std::make_unique<Sprite>();
		sSelectButtons_[i]->SetPosition({ 250.0f, 525.0f + (i * 50.0f) });
		sSelectButtons_[i]->SetAnchorPoint({ 0.5f, 0.5f });
		sSelectButtons_[i]->SetSize({ 310.0f, 40.0f });
	}

	sSelectButtonFrame_ = std::make_unique<Sprite>();
	sSelectButtonFrame_->SetAnchorPoint({ 0.5f, 0.5f });
	sSelectButtonFrame_->SetSize({ 324.0f, 54.0f });
#pragma endregion

#pragma region テクスチャハンドル
	hTitle_ = LoadTexture("Resources/Title.png");
	gSelectButton_ = LoadTexture("Resources/titleSelectButton.png");
	gSelectButtonFrame_ = LoadTexture("Resources/titleSelectButtonFrame.png");
#pragma endregion

#pragma region コライダー
	mouseCol_ = std::make_unique<PointCollider>();
	mouseCol_->SetAttribute(COL_POINT);
	colMgr2D_->AddCollider(mouseCol_.get());

	selectButtonsCol_.resize(2);
	for (uint16_t i = 0; i < selectButtonsCol_.size(); i++) {
		selectButtonsCol_[i] = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, Vector2{ 155.0f, 20.0f });
		selectButtonsCol_[i]->SetSprite(sSelectButtons_[i].get());
		selectButtonsCol_[i]->SetAttribute(COL_BOX);
		colMgr2D_->AddCollider(selectButtonsCol_[i].get());
	}
#pragma endregion
}

void TitleScene::Update()
{
	// マウスのコライダー更新
	mouseCol_->SetOffset(mouse_->MousePos());

	// 衝突処理2D
	OnCollision();

	// 行列更新処理
	MatUpdate();

	if (key_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->SceneTransition(GAME);
	}

	ImGui::Begin("Debug");
	ImGui::Text("MousePos = {%f, %f}", mouseCol_->GetOffset().x, mouseCol_->GetOffset().y);
	ImGui::End();
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Sprite");

	// タイトルを描画
	//sTitle_->Draw(hTitle_);

	// セレクトボタン描画
	for (auto& it : sSelectButtons_) it->Draw(gSelectButton_);

	// セレクトボタンの枠
	sSelectButtonFrame_->Draw(gSelectButtonFrame_);
}

void TitleScene::OnCollision()
{
	// 衝突全チェック
	colMgr2D_->CheckAllCollision();

	for (uint16_t i = 0; i < selectButtonsCol_.size();i++) {
		sSelectButtons_[i]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		if (selectButtonsCol_[i]->GetIsHit()) {
			sSelectButtons_[i]->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
		}
	}
}

void TitleScene::MatUpdate()
{
	// カメラ更新
	camera_->Update();

	// タイトル更新
	sTitle_->MatUpdate();

	// セレクトボタン更新
	for (auto& it : sSelectButtons_) it->MatUpdate();

	// セレクトボタンの枠
	sSelectButtonFrame_->MatUpdate();
}