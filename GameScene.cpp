#include "GameScene.h"
#include "FumiEngine.h"

bool CirBoxCol(Circle cir, RectAngle box) {
	float boxX1 = box.x - box.rX;
	float boxX2 = box.x + box.rX;
	float boxY1 = box.y - box.rY;
	float boxY2 = box.y + box.rY;

	if ((boxX1 < cir.x) && (cir.x < boxX2) && (boxY1 - cir.r < cir.y) && (cir.y < boxY2 + cir.r)) return true;
	if ((boxX1 - cir.r < cir.x) && (cir.x < boxX2 + cir.r) && (boxY1 < cir.y) && (cir.y < boxY2)) return true;
	if (pow((double)boxX1 - cir.x, 2) + pow((double)boxY1 - cir.y, 2) < pow(cir.r, 2)) return true;
	if (pow((double)boxX1 - cir.x, 2) + pow((double)boxY2 - cir.y, 2) < pow(cir.r, 2)) return true;
	if (pow((double)boxX2 - cir.x, 2) + pow((double)boxY1 - cir.y, 2) < pow(cir.r, 2)) return true;
	if (pow((double)boxX2 - cir.x, 2) + pow((double)boxY2 - cir.y, 2) < pow(cir.r, 2)) return true;
	return false;
}

// �R���X�g���N�^
GameScene::GameScene() :
	key_(nullptr),// -> �L�[�{�[�h����

	camera_(nullptr),// -> �J����

	player_(nullptr),// -> �v���C���[

	stage_(nullptr),// -> �X�e�[�W

	// ���f��
	blackFloorM_(nullptr),// -> ���F�̏�
	whiteFloorM_(nullptr),// -> ���F�̏�
	wallM_(nullptr),// -> ��

	// �I�u�W�F�N�g
	floor_{}// -> ��
{

}

// �f�X�g���N�^
GameScene::~GameScene() {
	delete camera_;
	delete player_;
	delete stage_;
	delete blackFloorM_;
	delete whiteFloorM_;
	delete wallM_;
	for (size_t i = 0; i < maxFloor_; i++) delete floor_[i];
}

// ����������
void GameScene::Initialize() {
	// �L�[�{�[�h����
	key_ = Key::GetInstance();

	// ���f��
	blackFloorM_ = Model::CreateModel("blackFloor");// -> ���F�̏�
	whiteFloorM_ = Model::CreateModel("whiteFloor");// -> ���F�̏�
	wallM_ = Model::CreateModel("wall");// -> ��

	// �J����
	camera_ = new Camera();
	camera_->eye_ = { -10.0f, 5.0f, 0.0f };
	camera_->target_ = { 0.0f, 5.0f, 10.0f };

	// �I�u�W�F�N�g
	for (size_t i = 0; i < maxFloor_; i++) {
		floor_[i] = Object3D::CreateObject3D();
		floor_[i]->scale_ = { 10.0f, 10.0f, 10.0f };
		floor_[i]->position_ = { 5.0f + (10.0f * (i % 13)), 0.0f, -5.0f + (-10.0f * (i / 13)) };
		floor_[i]->SetCamera(camera_);
		if ((i / 13) % 2 == 0 ) {
			if (i % 2 == 0) floor_[i]->SetModel(blackFloorM_);
			if (i % 2 == 1) floor_[i]->SetModel(whiteFloorM_);
		}
		else {
			if (i % 2 == 1) floor_[i]->SetModel(whiteFloorM_);
			if (i % 2 == 0) floor_[i]->SetModel(blackFloorM_);
		}
	}

	// �v���C���[����������
	player_ = new Player();
	player_->Initialize();
	player_->SetCamera(camera_);

	stage_ = new Stage();
	stage_->SetCamera(camera_);
	stage_->Initialize();
}

// �X�V����
void GameScene::Update() {
	// �v���C���[�X�V����
	player_->Update();

	// �����蔻�菈��
	Collision();

	// �J�����̍X�V
	camera_->Update();
}

// �`�揈��
void GameScene::Draw() {

	// ���f���`��O����
	Object3D::PreDraw();

	// �v���C���[���f���`��
	for (size_t i = 0; i < maxFloor_; i++) floor_[i]->Draw();

	// �v���C���[�`�揈��
	player_->Draw();

	stage_->Draw();
}

void GameScene::Collision() {
	for (size_t i = 0; i < stage_->wallsObj_.size(); i++) {

		if (CirBoxCol(player_->col_, stage_->wallsCol_[i])) {
			camera_->eye_.x = player_->oldCol_.x;
			camera_->eye_.z = player_->oldCol_.y;
			player_->col_ = player_->oldCol_;
		}
	}
}