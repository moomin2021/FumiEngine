#include "TitleScene.h"
#include "Texture.h"
#include "PipelineManager.h"
#include "SceneManager.h"
#include "CollisionAttribute.h"
#include "Util.h"
#include "Easing.h"

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

#pragma region ライトグループ
	lightGroup_ = std::make_unique<LightGroup>();
	Object3D::SetLightGroup(lightGroup_.get());

	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 20.0f, -90.0f });
	camera_->SetTarget({ 0.0f, 10.0f, 0.0f });
	Sprite::SetCamera(camera_.get());
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region スプライト
	sTitle_ = std::make_unique<Sprite>();
	sTitle_->SetSize({ 1920.0f, 1080.0f });

	sSelectButtons_.resize(3);
	for (uint16_t i = 0; i < sSelectButtons_.size(); i++) {
		sSelectButtons_[i] = std::make_unique<Sprite>();
		sSelectButtons_[i]->SetPosition({ 250.0f, 525.0f + (i * 50.0f) });
		sSelectButtons_[i]->SetAnchorPoint({ 0.5f, 0.5f });
		sSelectButtons_[i]->SetSize({ 310.0f, 40.0f });
	}

	sSelectButtonFrame_ = std::make_unique<Sprite>();
	sSelectButtonFrame_->SetAnchorPoint({ 0.5f, 0.5f });
	sSelectButtonFrame_->SetSize({ 324.0f, 54.0f });

	sSelectText_.resize(3);
	for (uint16_t i = 0; i < sSelectText_.size(); i++) {
		sSelectText_[i] = std::make_unique<Sprite>();
		sSelectText_[i]->SetPosition({ 250.0f, 525.0f + (i * 50.0f) });
		sSelectText_[i]->SetAnchorPoint({ 0.5f, 0.5f });
		sSelectText_[i]->SetSize({ 310.0f, 40.0f });
	}
#pragma endregion

#pragma region テクスチャハンドル
	gTitle_ = LoadTexture("Resources/title.png");
	gSelectButton_ = LoadTexture("Resources/titleSelectButton.png");
	gSelectButtonFrame_ = LoadTexture("Resources/titleSelectButtonFrame.png");

	gSelectText_.resize(3);
	gSelectText_[0] = LoadTexture("resources/titleSelectText0.png");
	gSelectText_[1] = LoadTexture("resources/titleSelectText1.png");
	gSelectText_[2] = LoadTexture("resources/titleSelectText2.png");
#pragma endregion

#pragma region コライダー
	mouseCol_ = std::make_unique<PointCollider>();
	mouseCol_->SetAttribute(COL_POINT);
	colMgr2D_->AddCollider(mouseCol_.get());

	selectButtonsCol_.resize(3);
	for (uint16_t i = 0; i < selectButtonsCol_.size(); i++) {
		selectButtonsCol_[i] = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, Vector2{ 155.0f, 20.0f });
		selectButtonsCol_[i]->SetSprite(sSelectButtons_[i].get());
		selectButtonsCol_[i]->SetAttribute(COL_BOX);
		colMgr2D_->AddCollider(selectButtonsCol_[i].get());
	}
#pragma endregion

#pragma region ボタン関連
	selectButtonPos_.resize(3);
	for (uint8_t i = 0; i < selectButtonPos_.size();i++) {
		selectButtonPos_[i] = {250.0f, 525.0f + (i * 50.0f)};
	}
#pragma endregion

#pragma region ステージ
	stage_ = std::make_unique<Stage>();
	stage_->Initialize();
	stage_->Load("Resources/StageJson/stage1.json", false);
#pragma endregion
}

void TitleScene::Update()
{
	// セレクトボタンの処理
	SelectButton();

	// カメラ回転
	CameraRota();

	// マウスのコライダー更新
	mouseCol_->SetOffset(mouse_->MousePos());

	// 衝突処理2D
	OnCollision();

	// 行列更新処理
	MatUpdate();

	ImGui::Begin("Debug");
	ImGui::Text("MousePos = {%f, %f}", mouseCol_->GetOffset().x, mouseCol_->GetOffset().y);
	ImGui::Text("nowSelect = %d", nowSelect_);
	ImGui::Text("isSelect = %d", isSelect_);
	ImGui::Text("isHit0 = %d", selectButtonsCol_[0]->GetIsHit());
	ImGui::Text("isHit1 = %d", selectButtonsCol_[1]->GetIsHit());
	ImGui::End();

	if (mouse_->TriggerMouseButton(M_LEFT)) {
		if (nowSelect_ == SelectNum::START) {
			SceneManager::GetInstance()->SceneTransition(GAME);
		}

		else if (nowSelect_ == SelectNum::SETTING)
		{

		}

		else if (nowSelect_ == SelectNum::END) {
			SceneManager::GetInstance()->SetIsEnd(true);
		}
	}
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	stage_->Draw();

	PipelineManager::PreDraw("Sprite");

	// タイトルを描画
	sTitle_->Draw(gTitle_);

	// セレクトボタン描画
	for (auto& it : sSelectButtons_) it->Draw(gSelectButton_);

	// セレクトボタンの枠
	if (isSelect_) sSelectButtonFrame_->Draw(gSelectButtonFrame_);

	// セレクトテキスト
	for (uint16_t i = 0; i < sSelectText_.size(); i++) sSelectText_[i]->Draw(gSelectText_[i]);
}

void TitleScene::OnCollision()
{
	// 衝突全チェック
	colMgr2D_->CheckAllCollision();

#pragma region ボタンとマウスの判定
	bool result = false;
	for (uint16_t i = 0; i < selectButtonsCol_.size();i++) {
		sSelectButtons_[i]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		if (selectButtonsCol_[i]->GetIsHit()) {
			if (isSelect_ == false) startEaseTime_ = Util::GetTimrMSec();// イージング開始時間を記録
			if ((uint16_t)nowSelect_ != i) startEaseTime_ = Util::GetTimrMSec();// イージング開始時間を記録
			result = true;// 選択中フラグを[ON]
			sSelectButtons_[i]->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });// 色を変える
			nowSelect_ = (SelectNum)i;// 選択しているものを保存
			sSelectButtonFrame_->SetPosition(selectButtonPos_[i]);
		}
	}

	isSelect_ = result;
	if (isSelect_ == false) nowSelect_ = SelectNum::NONE;
#pragma endregion
}

void TitleScene::MatUpdate()
{
	// カメラ更新
	camera_->Update();

	// ステージ
	stage_->MatUpdate();

	// タイトル
	sTitle_->MatUpdate();

	// セレクトボタン更新
	for (auto& it : sSelectButtons_) it->MatUpdate();

	// セレクトボタンの枠
	sSelectButtonFrame_->MatUpdate();

	// セレクトテキスト
	for (auto& it : sSelectText_) it->MatUpdate();
}

void TitleScene::SelectButton()
{
	if (isSelect_) {
		float elapsedTime = (Util::GetTimrMSec() - startEaseTime_) / 1000.0f;
		float rate = elapsedTime / easeTime_;
		rate = Util::Clamp(rate, 1.0f, 0.0f);
		Vector2 resultPos = { 0.0f, 0.0f };
		resultPos.x = Easing::Quint::easeOut(startSelectButtonFrameSize_.x, endSelectButtonFrameSize_.x, rate);
		resultPos.y = Easing::Quint::easeOut(startSelectButtonFrameSize_.y, endSelectButtonFrameSize_.y, rate);
		sSelectButtonFrame_->SetSize(resultPos);
	}
}

void TitleScene::CameraRota()
{
	static float angle = 0.0f;
	angle += 0.1f;

	Vector3 cameraPos = {
		sinf(Util::Degree2Radian(angle)),
		0.0f,
		cosf(Util::Degree2Radian(angle))
	};

	cameraPos.normalize();

	cameraPos *= 90.0f;
	cameraPos.y = 10.0f;

	camera_->SetEye(cameraPos);
}