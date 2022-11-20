#include "Scene1.h"
#include "FumiEngine.h"

Scene1::Scene1() :
#pragma region 初期化リスト
	camera_(nullptr),// -> カメラ
	object_(nullptr)// -> オブジェクト
#pragma endregion
{

}

Scene1::~Scene1() {
	delete camera_;
	delete object_;
}

void Scene1::Initialize() {
	// キーボードクラスインスタンス取得
	key_ = Key::GetInstance();

	// カメラ生成
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 0.0f, -10.0f };

	// オブジェクト生成
	object_ = new Object3D();
	object_->CreateCube();
	object_->position_ = { -3.0f, 0.0f, 0.0f };

	// 画像ハンドル取得
	valoHandle_ = LoadTexture(L"Resources/valo.jpg");
}

void Scene1::Update() {
	// カメラ移動
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.y += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);
	camera_->eye_.z += key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN);

	// カメラの更新処理
	camera_->Update();

	// オブジェクト更新処理
	object_->Update(camera_);
}

void Scene1::Draw() {
	// オブジェクト描画前処理
	Object3D::PreDraw();

	// オブジェクト描画処理
	object_->Draw(valoHandle_);
}