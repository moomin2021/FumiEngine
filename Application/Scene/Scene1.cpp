#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "CollisionManager.h"

#include <DirectXMath.h>

using namespace DirectX;

bool SphereCollision(const SphereCol& s0, const SphereCol& s1) {
	float a = s0.pos.x - s1.pos.x;
	float b = s0.pos.y - s1.pos.y;
	float c = s0.pos.z - s1.pos.z;
	float d = sqrtf(a * a + b * b + c * c);
	if (d <= s0.radius + s1.radius) return true;
	return false;
}

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

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// モデル
	mFloor_ = std::make_unique<Model>("floor");
	mCube_ = std::make_unique<Model>("cube");
	mSphere_ = std::make_unique<Model>("sphere");

	// オブジェクト
	oFloor_ = std::make_unique<Object3D>(mFloor_.get());
	oFloor_->SetScale({ 10.0f, 10.0f, 10.0f });

	oSphere_ = std::make_unique<Object3D>(mSphere_.get());
	oSphere_->SetPosition({ 0.0f, 1.0f, -3.0f });
	oSphere_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	oCube_.resize(3);

	oCube_[0] = std::make_unique<Object3D>(mCube_.get());
	oCube_[0]->SetPosition({ 3.0f, 1.0f, 0.0f });

	oCube_[1] = std::make_unique<Object3D>(mCube_.get());
	oCube_[1]->SetPosition({ -3.0f, 1.0f, 0.0f });

	oCube_[2] = std::make_unique<Object3D>(mCube_.get());
	oCube_[2]->SetPosition({ 0.0f, 1.0f, 0.0f });

	// テクスチャハンドル
	haeHandle_ = LoadTexture("Resources/hae.png");

	// スプライト
	sHae_ = std::make_unique<Sprite>();
	sHae_->SetPosition({ 0.0f, 0.0f });

	// ライト生成
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	//pointLight_->SetLightPos({ -4.0f, 1.0f, 0.0f });

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

	// カメラ移動
	{
		static float3 eye = { 0.0f, 10.0f, -30.0f };
	// ステージ更新
	loadStage_->Update();

	// プレイヤー更新
	player_->Update();

	Collision();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// プレイヤーの描画処理
	player_->Draw();

	// ステージ描画
	loadStage_->Draw();

	PipelineManager::PreDraw("Sprite");

	player_->Draw2D();
}

void Scene1::Collision()
{
	PipelineManager::GetInstance()->PreDraw("Object3D");

	oFloor_->Draw();
	oSphere_->Draw();
	for (auto& object : oCube_) object->Draw();

	PipelineManager::GetInstance()->PreDraw("Sprite");

	//sHae_->Draw();
	// 弾と敵の衝突判定
	for (size_t i = 0; i < loadStage_->enemys_.size(); i++) {
		if (loadStage_->enemys_[i]->GetIsAlive()) {
			for (size_t j = 0; j < player_->bullets_.size(); j++) {
				if (SphereCollision(loadStage_->enemys_[i]->GetSphereCol(), player_->bullets_[j]->GetSphereCol())) {
				loadStage_->enemys_[i]->ReduceHP(1);
					player_->bullets_.erase(player_->bullets_.begin() + j);
				}
			}
		}
	}
}