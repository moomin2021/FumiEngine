#include "GameScene.h"
#include "FumiEngine.h"

bool CirBoxCol(float cirX, float cirY, float cirR, float boxX1, float boxY1, float boxX2, float boxY2) {
	if ((boxX1 < cirX) && (cirX < boxX2) && (boxY1 - cirR < cirY) && (cirY < boxY2 + cirR)) return true;
	if ((boxX1 - cirR < cirX) && (cirX < boxX2 + cirR) && (boxY1 < cirY) && (cirY < boxY2)) return true;
	if (pow((double)boxX1 - cirX, 2) + pow((double)boxY1 - cirY, 2) < pow(cirR, 2)) return true;
	if (pow((double)boxX1 - cirX, 2) + pow((double)boxY2 - cirY, 2) < pow(cirR, 2)) return true;
	if (pow((double)boxX2 - cirX, 2) + pow((double)boxY1 - cirY, 2) < pow(cirR, 2)) return true;
	if (pow((double)boxX2 - cirX, 2) + pow((double)boxY2 - cirY, 2) < pow(cirR, 2)) return true;
	return false;
}

bool PointBoxCol(float pX, float pY, float boxX1, float boxY1, float boxX2, float boxY2) {
	if (pX > boxX1 || pX < boxX2) return false;
	if (pY > boxY1 || pY < boxY2) return false;
	return true;
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
	for (size_t i = 0; i < maxWall_; i++) delete wallObj_[i];
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

	for (size_t i = 0; i < maxWall_; i++) {
		wallObj_[i] = Object3D::CreateObject3D();
		wallObj_[i]->position_ = { -20.0f + (i * 10.0f), 5.0f, 10.0f };
		wallObj_[i]->scale_ = { 10.0f, 10.0f, 10.0f };
		wallObj_[i]->rotation_.y = -90.0f;
		wallObj_[i]->SetCamera(camera_);
		wallObj_[i]->SetModel(wallM_);
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

	//for (size_t i = 0; i < maxWall_; i++) wallObj_[i]->Draw();

	// �v���C���[�`�揈��
	player_->Draw();

	stage_->Draw();
}

void GameScene::Collision() {
	for (size_t i = 0; i < stage_->wallsObj_.size(); i++) {
		if (stage_->wallsObj_[i].rotation_.y >= 90.0f) {
			if (CirBoxCol(camera_->eye_.x, camera_->eye_.z, 1.0f,
				stage_->wallsObj_[i].position_.x - 5.0f, stage_->wallsObj_[i].position_.z - 0.5f,
				stage_->wallsObj_[i].position_.x + 5.0f, stage_->wallsObj_[i].position_.z + 0.5f)) {
				camera_->eye_.x = player_->oldPos_.x;
				camera_->eye_.y = player_->oldPos_.y;
				camera_->eye_.z = player_->oldPos_.z;
			}
		}
		else {
			if (CirBoxCol(camera_->eye_.x, camera_->eye_.z, 1.0f,
				stage_->wallsObj_[i].position_.x - 0.5f, stage_->wallsObj_[i].position_.z - 5.0f,
				stage_->wallsObj_[i].position_.x + 0.5f, stage_->wallsObj_[i].position_.z + 5.0f)) {
				camera_->eye_.x = player_->oldPos_.x;
				camera_->eye_.y = player_->oldPos_.y;
				camera_->eye_.z = player_->oldPos_.z;
			}
		}

		//if (PointBoxCol(camera_->eye_.x, camera_->eye_.z,
		//	wallObj_[i]->position_.x + 5.0f, wallObj_[i]->position_.z + 0.5f,
		//	wallObj_[i]->position_.x - 5.0f, wallObj_[i]->position_.z - 0.5f)) {
		//	camera_->eye_.x = player_->oldPos_.x;
		//	camera_->eye_.y = player_->oldPos_.y;
		//	camera_->eye_.z = player_->oldPos_.z;
		//}
	}
}