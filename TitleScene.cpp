#include "TitleScene.h"
#include "FumiEngine.h"

TitleScene::TitleScene() :
	key_(nullptr),
	player_(nullptr),
	object_(nullptr),
	camera_(nullptr),
	sprite_(nullptr)
{
}

TitleScene::~TitleScene()
{
	delete player_;
	delete object_;
	delete camera_;
	delete sprite_;
}

void TitleScene::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// プレイヤーモデル
	player_ = player_->CreateModel("player_rest");

	// オブジェクト
	object_ = Object3D::CreateObject3D();
	object_->SetCamera(camera_);
	object_->SetModel(player_);
}

void TitleScene::Update()
{
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.z += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);

	// カメラの更新
	camera_->Update();

	// オブジェクト更新
	object_->rotation_.y += 0.1f;
}

void TitleScene::Draw()
{
	// モデル描画前処理
	Object3D::PreDraw();

	// プレイヤーモデル描画
	object_->Draw();
}
