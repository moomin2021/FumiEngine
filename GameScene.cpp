#include "GameScene.h"
#include "FumiEngine.h"

// コンストラクタ
GameScene::GameScene() :
	key_(nullptr),// -> キーボード入力

	// モデル
	blackFloor_(nullptr),// -> 黒色の床

	// オブジェクト
	floor_{},// -> 床

	// 各オブジェクトの数
	maxFloor_(0)

	// カメラ
	camera_(nullptr)
{

}

// デストラクタ
GameScene::~GameScene() {
	delete player_;
	for (size_t i = 0; i < 10; i++) delete object_[i];
	delete camera_;
}

// 初期化処理
void GameScene::Initialize() {
	key_ = Key::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// プレイヤーモデル
	player_ = player_->CreateModel("player_rest");
	
	for (size_t i = 0; i < 10; i++) {
		object_[i] = Object3D::CreateObject3D();
		object_[i]->position_ = { -10.0f + (2.0f * i), 0.0f, 0.0f };
		object_[i]->SetCamera(camera_);
		object_[i]->SetModel(player_);
	}
}

// 更新処理
void GameScene::Update() {
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.z += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);

	// カメラの更新
	camera_->Update();

	for (size_t i = 0; i < 10; i++) {
		object_[i]->rotation_.y += 0.1f;
	}
}

// 描画処理
void GameScene::Draw() {

	// モデル描画前処理
	Object3D::PreDraw();

	// プレイヤーモデル描画
	for (size_t i = 0; i < 10; i++) {
		object_[i]->Draw();
	}
}