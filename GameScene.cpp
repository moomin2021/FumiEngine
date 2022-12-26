#include "GameScene.h"
#include "FumiEngine.h"

// �R���X�g���N�^
GameScene::GameScene() :
	key_(nullptr),
	player_(nullptr),
	object_(nullptr),
	camera_(nullptr)
{

}

// �f�X�g���N�^
GameScene::~GameScene() {
	delete player_;
	delete object_;
	delete camera_;
}

// ����������
void GameScene::Initialize() {
	key_ = Key::GetInstance();

	// �J����
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// �v���C���[���f��
	player_ = player_->CreateModel("player_rest");

	object_ = object_->CreateObject3D();
	object_->SetCamera(camera_);
	object_->SetModel(player_);
}

// �X�V����
void GameScene::Update() {
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.z += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);

	// �J�����̍X�V
	camera_->Update();

	object_->rotation_.y += 0.1f;
}

// �`�揈��
void GameScene::Draw() {

	// ���f���`��O����
	Object3D::PreDraw();

	// �v���C���[���f���`��
	object_->Draw();
}