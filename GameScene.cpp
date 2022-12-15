#include "GameScene.h"
#include "FumiEngine.h"

// �R���X�g���N�^
GameScene::GameScene() :
	sprite_(nullptr),// -> �X�v���C�g
	model_(nullptr),
	camera_(nullptr)
{

}

// �f�X�g���N�^
GameScene::~GameScene() {
	delete sprite_;// -> �X�v���C�g
	delete model_;
	delete camera_;
}

// ����������
void GameScene::Initialize() {
	// �X�v���C�g
	sprite_ = new Sprite();
	model_ = new Model();
	model_->CreateModel("player_rest");
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };
	textureHandle_ = LoadTexture("Resources/valo.jpg");
	object_ = new Object3D();
	object_->CreateModel();
}

// �X�V����
void GameScene::Update() {
	camera_->Update();

	sprite_->Update();
	model_->Update(camera_);
	object_->Update(camera_);
}

// �`�揈��
void GameScene::Draw() {
	Sprite::PreDraw();
	sprite_->Draw(textureHandle_);

	Model::PreDraw();
	model_->Draw();

	Object3D::PreDraw();
	//object_->Draw();
}