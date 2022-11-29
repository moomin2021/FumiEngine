#include "GameScene.h"

// コンストラクタ
GameScene::GameScene() :
	key_(nullptr),// -> キー入力
	sprite_(nullptr)// -> スプライト
{

}

// デストラクタ
GameScene::~GameScene() {
	delete sprite_;// -> スプライト
}

// 初期化処理
void GameScene::Initialize() {
	// インスタンス取得
	key_ = Key::GetInstance();

	// スプライト
	sprite_ = new Sprite();

	// 描画フラグ
	isDraw_ = false;
}

// 更新処理
void GameScene::Update() {
	// [SPACE]キーを押したら描画フラグを切り替える
	if (key_->TriggerKey(DIK_SPACE)) isDraw_ = !isDraw_;

	// スプライト移動処理
	sprite_->position.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	sprite_->position.y += key_->PushKey(DIK_S) - key_->PushKey(DIK_W);

	// スプライト更新処理
	sprite_->Update();
}

// 描画処理
void GameScene::Draw() {
	Sprite::PreDraw();
	// 描画フラグが[ON]なら描画
	if (isDraw_) sprite_->Draw();
}