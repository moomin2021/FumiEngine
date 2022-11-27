#include "GameScene.h"

// コンストラクタ
GameScene::GameScene() :
	sprite_(nullptr)// -> スプライト
{

}

// デストラクタ
GameScene::~GameScene() {
	delete sprite_;// -> スプライト
}

// 初期化処理
void GameScene::Initialize() {
	// スプライト
	sprite_ = new Sprite();
}

// 更新処理
void GameScene::Update() {
	sprite_->Update();
}

// 描画処理
void GameScene::Draw() {
	Sprite::PreDraw();
	sprite_->Draw();
}