#include "GameScene.h"
#include "FumiEngine.h"

// コンストラクタ
GameScene::GameScene() :
	key_(nullptr),
	player_(nullptr),
	cube_(nullptr),
	camera_(nullptr)
{

}

// デストラクタ
GameScene::~GameScene() {
	delete player_;
	delete cube_;
	delete camera_;
}

// 初期化処理
void GameScene::Initialize() {
	key_ = Key::GetInstance();

	// プレイヤーモデル
	player_ = new Model();
	player_->CreateModel("player_rest");
	player_->position_ = { -10.0f, 0.0f, 0.0f };

	// キューブモデル
	cube_ = new Model();
	cube_->CreateModel("cube");
	cube_->position_ = { 10.0f, 0.0f, 0.0f };

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };
}

// 更新処理
void GameScene::Update() {
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.z += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);

	// カメラの更新
	camera_->Update();

	// プレイヤーモデルの更新
	player_->Update(camera_);

	// キューブモデルの更新
	cube_->Update(camera_);
}

// 描画処理
void GameScene::Draw() {

	// モデル描画前処理
	Model::PreDraw();

	// プレイヤーモデル描画
	player_->Draw();

	// キューブモデル描画
	cube_->Draw();
}