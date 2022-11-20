#include "Scene2.h"
#include "FumiEngine.h"
#include <time.h>

Scene2::Scene2() :
#pragma region 初期化リスト
	camera_(nullptr),// -> カメラ
	billBoard_{}// -> ビルボード
#pragma endregion
{

}

Scene2::~Scene2() {
	delete camera_;
	for (size_t i = 0; i < 10; i++) {
		delete billBoard_[i];
	}
}

void Scene2::Initialize() {
	srand(time(nullptr));

	// キーボードクラスインスタンス取得
	key_ = Key::GetInstance();

	// カメラ生成
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 0.0f, -10.0f };

	// ビルボード生成
	for (size_t i = 0; i < 10; i++) {
		billBoard_[i] = new BillBoard();
		billBoard_[i]->position_.x = (rand() % 6 - 2) * 0.5f;
		billBoard_[i]->position_.z = (rand() % 6 - 2) * 0.5f;
		billBoard_[i]->position_.z = (rand() % 6 - 2) * 0.5f;
	}

	// 画像ハンドル取得
	valoHandle_ = LoadTexture(L"Resources/kusa.png");
}

void Scene2::Update() {
	if (key_->TriggerKey(DIK_R)) {
		camera_->eye_ = { 0.0f, 0.0f, -10.0f };
	}

	// カメラ移動
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.y += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);
	camera_->eye_.z += key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN);

	// カメラの更新処理
	camera_->Update();

	// ビルボード更新処理
	for (size_t i = 0; i < 10; i++) {
		billBoard_[i]->Update(camera_, BillBoardType::BILLBOARDY);
	}
}

void Scene2::Draw() {
	// オブジェクト描画前処理
	BillBoard::PreDraw();

	// ビルボード描画処理
	for (size_t i = 0; i < 10; i++) {
		billBoard_[i]->Draw(valoHandle_);
	}
}