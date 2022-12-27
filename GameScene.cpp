#include "GameScene.h"
#include "FumiEngine.h"

// コンストラクタ
GameScene::GameScene() :
	key_(nullptr),// -> キーボード入力

	// モデル
	blackFloor_(nullptr),// -> 黒色の床
	whiteFloor_(nullptr),// -> 白色の床

	// オブジェクト
	floor_{},// -> 床

	// カメラ
	camera_(nullptr)
{

}

// デストラクタ
GameScene::~GameScene() {
	delete blackFloor_;
	delete whiteFloor_;
	for (size_t i = 0; i < maxFloor_; i++) delete floor_[i];
	delete camera_;
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
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// オブジェクト
	for (size_t i = 0; i < maxFloor_; i++) {
		floor_[i] = Object3D::CreateObject3D();
		floor_[i]->position_ = { -4.5f + (1.0f * (i % 10)), 0.0f, -4.5f + (1.0f * (i / 10)) };
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
}

// 更新処理
void GameScene::Update() {
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.z += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);

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
}