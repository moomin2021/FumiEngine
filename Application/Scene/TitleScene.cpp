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
	colMgr2D_->RemoveCollider(cMouse_.get());
}

void TitleScene::Initialize()
{
#pragma region インスタンス取得
	colMgr2D_ = CollisionManager2D::GetInstance();// 衝突マネージャー2D
	mouse_ = Mouse::GetInstance();
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

#pragma region ステージ
	stage_ = std::make_unique<Stage>();
	stage_->Initialize();
	stage_->Load("Resources/StageJson/stage1.json", false);
#pragma endregion

#pragma region コライダー
	cMouse_ = std::make_unique<PointCollider>();
	cMouse_->SetAttribute(COL_POINT);
	colMgr2D_->AddCollider(cMouse_.get());
#pragma endregion

#pragma region タイトルレイヤー
	titleLayer_ = std::make_unique<TitleLayer>();
	titleLayer_->Initialize();
#pragma endregion
}

void TitleScene::Update()
{
	// コライダーとマウスの座標をリンク
	cMouse_->SetOffset(mouse_->MousePos());

	// タイトルレイヤー
	titleLayer_->Update();

	// カメラ回転
	CameraRota();

	// 衝突処理2D
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	stage_->Draw();

	PipelineManager::PreDraw("Sprite");

	// タイトルレイヤー
	titleLayer_->Draw();
}

void TitleScene::OnCollision()
{
	// 衝突全チェック
	colMgr2D_->CheckAllCollision();

	// タイトルレイヤー
	titleLayer_->OnCollision();

#pragma region 左クリックを押したら
	// 衝突していなかったらこれ以降の処理を飛ばす
	if (mouse_->TriggerMouseButton(M_LEFT) == false) return;

	// マウスのコライダーから衝突しているコライダーのタグを属性に変換して取得
	ButtonAttribute buttonAttr = (ButtonAttribute)cMouse_->GetHitCollider()->GetTag();

	// スタート
	if (buttonAttr == ButtonAttribute::START)
	{
		SceneManager::GetInstance()->SceneTransition(SCENE::GAME);
	}
#pragma endregion
}

void TitleScene::MatUpdate()
{
	// カメラ更新
	camera_->Update();

	// ステージ
	stage_->MatUpdate();

	// タイトルレイヤー
	titleLayer_->MatUpdate();
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