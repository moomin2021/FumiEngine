#include "GameScene.h"
#include "FumiEngine.h"

// �R���X�g���N�^
GameScene::GameScene() :
	key_(nullptr),
	player_(nullptr),
	cube_(nullptr),
	camera_(nullptr)
{

}

// �f�X�g���N�^
GameScene::~GameScene() {
	delete player_;
	delete cube_;
	delete camera_;
}

// ����������
void GameScene::Initialize() {
	key_ = Key::GetInstance();

	// �v���C���[���f��
	player_ = new Model();
	player_->CreateModel("player_rest");
	player_->position_ = { -10.0f, 0.0f, 0.0f };

	// �L���[�u���f��
	cube_ = new Model();
	cube_->CreateModel("cube");
	cube_->position_ = { 10.0f, 0.0f, 0.0f };

	// �J����
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };
}

// �X�V����
void GameScene::Update() {
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.z += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);

	// �J�����̍X�V
	camera_->Update();

	// �v���C���[���f���̍X�V
	player_->Update(camera_);

	// �L���[�u���f���̍X�V
	cube_->Update(camera_);
}

// �`�揈��
void GameScene::Draw() {

	// ���f���`��O����
	Model::PreDraw();

	// �v���C���[���f���`��
	player_->Draw();

	// �L���[�u���f���`��
	cube_->Draw();
}