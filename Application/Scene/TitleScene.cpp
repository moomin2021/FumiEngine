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

	sSelectButtonFrame_ = std::make_unique<Sprite>();
	sSelectButtonFrame_->SetAnchorPoint({ 0.5f, 0.5f });
	sSelectButtonFrame_->SetSize({ 324.0f, 54.0f });
#pragma endregion

#pragma region テクスチャハンドル
	gTitle_ = LoadTexture("Resources/title.png");
	gSelectButtonFrame_ = LoadTexture("Resources/titleSelectButtonFrame.png");
#pragma endregion

#pragma region コライダー
	mouseCol_ = std::make_unique<PointCollider>();
	mouseCol_->SetAttribute(COL_POINT);
	colMgr2D_->AddCollider(mouseCol_.get());
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

#pragma region タイトルレイヤー
	titleLayer_ = std::make_unique<TitleLayer>();
	titleLayer_->Initialize();
#pragma endregion

#pragma region 設定レイヤー
	settingLayer_ = std::make_unique<SettingLayer>();
	settingLayer_->Initialize();
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

	// タイトルレイヤー
	titleLayer_->Update();

	// 設定レイヤー
	settingLayer_->Update();

	// 衝突処理2D
	OnCollision();

	// 行列更新処理
	MatUpdate();

	ImGui::Begin("Debug");
	ImGui::Text("MousePos = {%f, %f}", mouseCol_->GetOffset().x, mouseCol_->GetOffset().y);
	ImGui::Text("nowSelect = %d", nowSelect_);
	ImGui::Text("isSelect = %d", isSelect_);
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

	// セレクトボタンの枠
	if (isSelect_) sSelectButtonFrame_->Draw(gSelectButtonFrame_);

	// タイトルレイヤー
	titleLayer_->Draw();

	// 設定レイヤー
	settingLayer_->Draw();
}

void TitleScene::OnCollision()
{
	// 衝突全チェック
	colMgr2D_->CheckAllCollision();

#pragma region ボタンとマウスの判定
	bool result = false;
	//for (uint16_t i = 0; i < selectButtonsCol_.size();i++) {
	//	sSelectButtons_[i]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	//	if (selectButtonsCol_[i]->GetIsHit()) {
	//		if (isSelect_ == false) startEaseTime_ = Util::GetTimrMSec();// イージング開始時間を記録
	//		if ((uint16_t)nowSelect_ != i) startEaseTime_ = Util::GetTimrMSec();// イージング開始時間を記録
	//		result = true;// 選択中フラグを[ON]
	//		sSelectButtons_[i]->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });// 色を変える
	//		nowSelect_ = (SelectNum)i;// 選択しているものを保存
	//		sSelectButtonFrame_->SetPosition(selectButtonPos_[i]);
	//	}
	//}

	isSelect_ = result;
	if (isSelect_ == false) nowSelect_ = SelectNum::NONE;
#pragma endregion

	// タイトルレイヤー
	titleLayer_->OnCollision();

	// 設定レイヤー
	settingLayer_->OnCollision();
}

void TitleScene::MatUpdate()
{
	// カメラ更新
	camera_->Update();

	// ステージ
	stage_->MatUpdate();

	// タイトル
	sTitle_->MatUpdate();

	// セレクトボタンの枠
	sSelectButtonFrame_->MatUpdate();

	// タイトルレイヤー
	titleLayer_->MatUpdate();

	// 設定レイヤー
	settingLayer_->MatUpdate();
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