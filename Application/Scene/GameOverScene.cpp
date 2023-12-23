#include "GameOverScene.h"

#include "SceneManager.h"
#include "PipelineManager.h"
#include "WinAPI.h"
#include "Texture.h"

#include "CollisionAttribute.h"

GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {
	colMgr2D_->RemoveCollider(cMouse_.get());
}

void GameOverScene::Initialize()
{
	Vector2 winSize = 
	{	(float)WinAPI::GetInstance()->GetWidth(),
		(float)WinAPI::GetInstance()->GetHeight() };

#pragma region インスタンス
	colMgr2D_ = CollisionManager2D::GetInstance();
	BoxButton::SetCollisionManager2D(colMgr2D_);
	ResultBox::SetCollisionManager2D(colMgr2D_);

	mouse_ = Mouse::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 0.0f, -10.0f });
	Sprite::SetCamera(camera_.get());
#pragma endregion

#pragma region スプライト
	resultFrameS_ = std::make_unique<Sprite>();
	resultFrameS_->SetAnchorPoint({ 0.5f, 0.5f });
	resultFrameS_->SetPosition({ winSize.x / 2.0f, winSize.y / 2.0f });
	resultFrameS_->SetSize({ 1280.0f, 720.0f });
#pragma endregion

#pragma region 画像
	resultFrameH_ = LoadTexture("Sprite/resultFrame.png");
#pragma endregion

#pragma region ボタン
	titleReturnB_ = std::make_unique<BoxButton>();
	titleReturnB_->Initialize(1, { 1600.0f, 1000.0f }, { 260.0f, 44.0f }, {272.0f, 56.0f},
		LoadTexture("Sprite/treeButton.png"), LoadTexture("Sprite/titleReturnText.png"),
		LoadTexture("Sprite/treeButtonCursorFrame.png"));
#pragma endregion

#pragma region コライダー
	cMouse_ = std::make_unique<PointCollider>();
	cMouse_->SetAttribute(COL_POINT);
	colMgr2D_->AddCollider(cMouse_.get());
#pragma endregion

#pragma region リザルト表記
	resultBoxs_.resize(3);

	resultBoxs_[0] = std::make_unique<ResultBox>();
	resultBoxs_[0]->Initialize({ winSize.x / 2.0f, 250.0f }, {1258.0f, 44.0f},
		LoadTexture("Sprite/resultBox.png"), LoadTexture("Sprite/resultText0.png"));

	resultBoxs_[1] = std::make_unique<ResultBox>();
	resultBoxs_[1]->Initialize({ winSize.x / 2.0f, 300.0f }, { 1258.0f, 44.0f },
		LoadTexture("Sprite/resultBox.png"), LoadTexture("Sprite/resultText1.png"));

	resultBoxs_[2] = std::make_unique<ResultBox>();
	resultBoxs_[2]->Initialize({ winSize.x / 2.0f, 350.0f }, { 1258.0f, 44.0f },
		LoadTexture("Sprite/resultBox.png"), LoadTexture("Sprite/resultText2.png"));
#pragma endregion
}

void GameOverScene::Update()
{
	// コライダーとマウスの座標をリンク
	cMouse_->SetOffset(mouse_->MousePos());

	titleReturnB_->Update();
	
	for (auto& it : resultBoxs_) it->Update();

	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void GameOverScene::Draw()
{
	PipelineManager::PreDraw("Sprite");

	resultFrameS_->Draw(resultFrameH_);
	titleReturnB_->Draw();

	for (auto& it : resultBoxs_) it->Draw();
}

void GameOverScene::OnCollision()
{
	colMgr2D_->CheckAllCollision();

	titleReturnB_->OnCollision();

	for (auto& it : resultBoxs_) it->OnCollision();

	// 衝突していなかったらこれ以降の処理を飛ばす
	if (mouse_->TriggerMouseButton(M_LEFT) == false) return;

	// 取得するボタンの属性
	uint16_t buttonAttr = 0;

	// マウスのコライダーから衝突しているコライダーのタグを属性に変換して取得
	if (cMouse_->GetHitCollider() != nullptr)
	{
		buttonAttr = (uint16_t)cMouse_->GetHitCollider()->GetTag();
	}

	if (buttonAttr == 1)
	{
		SceneManager::GetInstance()->SceneTransition(SCENE::TITLE);
	}
}

void GameOverScene::MatUpdate()
{
	// カメラ
	camera_->Update();

	resultFrameS_->MatUpdate();
	titleReturnB_->MatUpdate();

	for (auto& it : resultBoxs_) it->MatUpdate();
}
