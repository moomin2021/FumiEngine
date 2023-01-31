#include "Scene2.h"
#include "FumiEngine.h"

using namespace std;

Scene2::Scene2() :
	key_(nullptr),
	camera_(nullptr),
	sprite_{}
{
}

Scene2::~Scene2() {}

void Scene2::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = make_unique<Camera>();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	sprite_[0] = make_unique<Sprite>();
	sprite_[1] = make_unique<Sprite>();

	texHandle_[0] = LoadTexture("Resources/valo.jpg");
	texHandle_[1] = LoadTexture("Resources/hae.png");
}

void Scene2::Update()
{
	static Float2 pos0 = { 0.0f, 0.0f };
	static Float2 pos1 = { 200.0f, 0.0f };

	pos0.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
	pos0.y += (key_->PushKey(DIK_S) - key_->PushKey(DIK_W)) * 0.5f;

	sprite_[0]->position = { pos0.x, pos0.y };
	sprite_[1]->position = { pos1.x, pos1.y };

	// スプライト更新
	for (auto& sprite : sprite_) sprite->Update();

	camera_->Update();
	// カメラの更新
}

void Scene2::Draw()
{
	Sprite::PreDraw();

	for (size_t i = 0; i < 2; i++) {
		sprite_[i]->Draw(texHandle_[i]);
	}
}