#include "GameScene.h"
#include "FumiEngine.h"

// �R���X�g���N�^
GameScene::GameScene() :
	key_(nullptr),// -> �L�[�{�[�h����

	camera_(nullptr),// -> �J����

	player_(nullptr),// -> �v���C���[

	// ���f��
	blackFloor_(nullptr),// -> ���F�̏�
	whiteFloor_(nullptr),// -> ���F�̏�

	// �I�u�W�F�N�g
	floor_{}// -> ��
{

}

// �f�X�g���N�^
GameScene::~GameScene() {
	delete camera_;
	delete player_;
	delete blackFloor_;
	delete whiteFloor_;
	for (size_t i = 0; i < maxFloor_; i++) delete floor_[i];
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
	camera_->eye_ = { 0.0f, 2.0f, 0.0f };
	camera_->target_ = { 0.0f, 2.0f, 10.0f };

	// �I�u�W�F�N�g
	for (size_t i = 0; i < maxFloor_; i++) {
		floor_[i] = Object3D::CreateObject3D();
		floor_[i]->scale_ = { 10.0f, 10.0f, 10.0f };
		floor_[i]->position_ = { -45.0f + (10.0f * (i % 10)), 0.0f, -45.0f + (10.0f * (i / 10)) };
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

	// �v���C���[����������
	player_ = new Player();
	player_->Initialize();
	player_->SetCamera(camera_);
}

// �X�V����
void GameScene::Update() {
	// �v���C���[�X�V����
	player_->Update();

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

	// �v���C���[�`�揈��
	player_->Draw();
}