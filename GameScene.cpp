#include "GameScene.h"
#include "FumiEngine.h"

// コンストラクタ
GameScene::GameScene() :
	key_(nullptr),
	player_(nullptr),
	object_(nullptr),
	camera_(nullptr)
{

}

// デストラクタ
GameScene::~GameScene() {
	delete player_;
	delete object_;
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

	object_ = object_->CreateObject3D();
	object_->SetCamera(camera_);
	object_->SetModel(player_);
}

// 更新処理
void GameScene::Update() {
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.z += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);

	// カメラの更新
	camera_->Update();

	object_->rotation_.y += 0.1f;
}

// 描画処理
void GameScene::Draw() {

	// モデル描画前処理
	Object3D::PreDraw();

	// プレイヤーモデル描画
	object_->Draw();
}