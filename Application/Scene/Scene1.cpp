#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "CollisionManager.h"

#include <DirectXMath.h>

using namespace DirectX;

Scene1::Scene1() :
	lightGroup_(nullptr),
	dirLight_(nullptr),

	// ステージ読み込み用
	loadStage_(nullptr),

	// プレイヤー
	player_(nullptr)
{
}

Scene1::~Scene1()
{
	
}

void Scene1::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// 敵管理クラスインスタンス取得
	enemyManager_ = EnemyManager::GetInstance();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// ライト生成
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });

	lightGroup_->AddDirLight(dirLight_.get());

	// ライトを適用
	Object3D::SetLightGroup(lightGroup_.get());

	// ステージ読み込み用クラス生成
	loadStage_ = std::make_unique<LoadStage>();

	// プレイヤー生成
	player_ = std::make_unique<Player>();
}

void Scene1::Update()
{
	// 衝突判定
	CollisionManager::GetInstance()->CheckAllCollision();

	// ステージ更新
	loadStage_->Update();

	// プレイヤー更新
	player_->Update();

	// 敵更新
	enemyManager_->Update();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// プレイヤーの描画処理
	player_->Draw();

	// ステージ描画
	loadStage_->Draw();

	// 敵描画
	enemyManager_->Draw();

	PipelineManager::PreDraw("Sprite");

	player_->Draw2D();
}