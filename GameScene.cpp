#include "GameScene.h"
#include "FumiEngine.h"

// コンストラクタ
GameScene::GameScene() :
	key_(nullptr),// -> キーボード入力

	camera_(nullptr),// -> カメラ

	player_(nullptr),// -> プレイヤー

	// モデル
	blackFloor_(nullptr),// -> 黒色の床
	whiteFloor_(nullptr),// -> 白色の床

	// オブジェクト
	floor_{}// -> 床
{

}

// デストラクタ
GameScene::~GameScene() {
	delete camera_;
	delete player_;
	delete blackFloor_;
	delete whiteFloor_;
	for (size_t i = 0; i < maxFloor_; i++) delete floor_[i];
}

// 初期化処理
void GameScene::Initialize() {
	// キーボード入力
	key_ = Key::GetInstance();

	// モデル
	blackFloor_ = Model::CreateModel("blackFloor");// -> 黒色の床
	whiteFloor_ = Model::CreateModel("whiteFloor");// -> 白色の床

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 2.0f, 0.0f };
	camera_->target_ = { 0.0f, 2.0f, 10.0f };

	// オブジェクト
	for (size_t i = 0; i < maxFloor_; i++) {
		floor_[i] = Object3D::CreateObject3D();
		floor_[i]->scale_ = { 10.0f, 10.0f, 10.0f };
		floor_[i]->position_ = { -45.0f + (10.0f * (i % 10)), 0.0f, -45.0f + (10.0f * (i / 10)) };
		floor_[i]->SetCamera(camera_);
		if ((i / 10) % 2 == 0 ) {
			if (i % 2 == 0) floor_[i]->SetModel(blackFloor_);
			if (i % 2 == 1) floor_[i]->SetModel(whiteFloor_);
		}
		else {
			if (i % 2 == 0) floor_[i]->SetModel(whiteFloor_);
			if (i % 2 == 1) floor_[i]->SetModel(blackFloor_);
		}
	}

	// プレイヤー初期化処理
	player_ = new Player();
	player_->Initialize();
	player_->SetCamera(camera_);
}

// 更新処理
void GameScene::Update() {
	// プレイヤー更新処理
	player_->Update();

	// カメラの更新
	camera_->Update();
}

// 描画処理
void GameScene::Draw() {

	// モデル描画前処理
	Object3D::PreDraw();

	// プレイヤーモデル描画
	for (size_t i = 0; i < maxFloor_; i++) {
		floor_[i]->Draw();
	}

	// プレイヤー描画処理
	player_->Draw();
}