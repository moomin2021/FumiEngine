#include "GameScene.h"
#include "FumiEngine.h"

// �R���X�g���N�^
GameScene::GameScene() :
	key_(nullptr),// -> �L�[�{�[�h����

	// ���f��
	blackFloor_(nullptr),// -> ���F�̏�

	// �I�u�W�F�N�g
	floor_{},// -> ��

	// �e�I�u�W�F�N�g�̐�
	maxFloor_(0)

	// �J����
	camera_(nullptr)
{

}

// �f�X�g���N�^
GameScene::~GameScene() {
	delete player_;
	for (size_t i = 0; i < 10; i++) delete object_[i];
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
	
	for (size_t i = 0; i < 10; i++) {
		object_[i] = Object3D::CreateObject3D();
		object_[i]->position_ = { -10.0f + (2.0f * i), 0.0f, 0.0f };
		object_[i]->SetCamera(camera_);
		object_[i]->SetModel(player_);
	}
}

// �X�V����
void GameScene::Update() {
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.z += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);

	// �J�����̍X�V
	camera_->Update();

	for (size_t i = 0; i < 10; i++) {
		object_[i]->rotation_.y += 0.1f;
	}
}

// �`�揈��
void GameScene::Draw() {

	// ���f���`��O����
	Object3D::PreDraw();

	// �v���C���[���f���`��
	for (size_t i = 0; i < 10; i++) {
		object_[i]->Draw();
	}
}