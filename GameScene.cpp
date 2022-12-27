#include "GameScene.h"
#include "FumiEngine.h"

// �R���X�g���N�^
GameScene::GameScene() :
	key_(nullptr),// -> �L�[�{�[�h����

	// ���f��
	blackFloor_(nullptr),// -> ���F�̏�
	whiteFloor_(nullptr),// -> ���F�̏�

	// �I�u�W�F�N�g
	floor_{},// -> ��

	// �J����
	camera_(nullptr)
{

}

// �f�X�g���N�^
GameScene::~GameScene() {
	delete blackFloor_;
	delete whiteFloor_;
	for (size_t i = 0; i < maxFloor_; i++) delete floor_[i];
	delete camera_;
}

// ����������
void GameScene::Initialize() {
	// �L�[�{�[�h����
	key_ = Key::GetInstance();

	// ���f��
	blackFloor_ = Model::CreateModel("blackFloor");// -> ���F�̏�
	whiteFloor_ = Model::CreateModel("whiteFloor");// -> ���F�̏�

	// �J����
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// �I�u�W�F�N�g
	for (size_t i = 0; i < maxFloor_; i++) {
		floor_[i] = Object3D::CreateObject3D();
		floor_[i]->position_ = { -4.5f + (1.0f * (i % 10)), 0.0f, -4.5f + (1.0f * (i / 10)) };
		floor_[i]->SetCamera(camera_);
		if ((i / 10) % 2 == 0 ) {
			if (i % 2 == 0) floor_[i]->SetModel(blackFloor_);
			if (i % 2 == 1) floor_[i]->SetModel(whiteFloor_);
		}
		else {
			if (i % 2 == 0) floor_[i]->SetModel(whiteFloor_);
			if (i % 2 == 1) floor_[i]->SetModel(blackFloor_);
		}
	}
}

// �X�V����
void GameScene::Update() {
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.z += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);

	// �J�����̍X�V
	camera_->Update();
}

// �`�揈��
void GameScene::Draw() {

	// ���f���`��O����
	Object3D::PreDraw();

	// �v���C���[���f���`��
	for (size_t i = 0; i < maxFloor_; i++) {
		floor_[i]->Draw();
	}
}