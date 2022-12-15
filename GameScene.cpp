#include "GameScene.h"
#include "FumiEngine.h"

// コンストラクタ
GameScene::GameScene() :
	sprite_(nullptr),// -> スプライト
	model_(nullptr),
	camera_(nullptr)
{

}

// デストラクタ
GameScene::~GameScene() {
	delete sprite_;// -> スプライト
	delete model_;
	delete camera_;
}

// 初期化処理
void GameScene::Initialize() {
	// スプライト
	sprite_ = new Sprite();
	model_ = new Model();
	model_->CreateModel("player_rest");
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };
	textureHandle_ = LoadTexture("Resources/valo.jpg");
	object_ = new Object3D();
	object_->CreateModel();
}

// 更新処理
void GameScene::Update() {
	camera_->Update();

	sprite_->Update();
	model_->Update(camera_);
	object_->Update(camera_);
}

// 描画処理
void GameScene::Draw() {
	Sprite::PreDraw();
	sprite_->Draw(textureHandle_);

	Model::PreDraw();
	model_->Draw();

	Object3D::PreDraw();
	//object_->Draw();
}