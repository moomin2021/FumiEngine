#include "GameScene.h"
#include "FumiEngine.h"

bool calcSegmentIntersectPos(Line2D line1, Line2D line2, Vector2* intersect) {
	Vector2 vec1 = line1.eP - line1.sP;
	Vector2 vec2 = line2.eP - line2.sP;

	float cV2V1 = vec2.cross(vec1);
	if (cV2V1 == 0.0f) return false;// -> ���s�Ȃ̂�[false]��Ԃ�

	Vector2 vec0 = line1.sP - line2.sP;
	float cV0V2 = vec0.cross(vec2);
	float t1 = cV0V2 / cV2V1;
	if (t1 < 0 || t1 > 1) return false;

	float cV0V1 = vec0.cross(vec1);
	float t2 = cV0V1 / cV2V1;
	if (t2 < 0 || t2 > 1) return false;

	*intersect = line1.sP + t1 * vec1;
	return true;
}

bool CirBoxCol(Circle2D cir, Box2D box) {
	float boxX1 = box.p.x - box.rX;
	float boxX2 = box.p.x + box.rX;
	float boxY1 = box.p.y - box.rY;
	float boxY2 = box.p.y + box.rY;

	if ((boxX1 <= cir.p.x) && (cir.p.x <= boxX2) && (boxY1 - cir.r <= cir.p.y) && (cir.p.y <= boxY2 + cir.r)) return true;
	if ((boxX1 - cir.r <= cir.p.x) && (cir.p.x <= boxX2 + cir.r) && (boxY1 <= cir.p.y) && (cir.p.y <= boxY2)) return true;
	if (pow((double)boxX1 - cir.p.x, 2) + pow((double)boxY1 - cir.p.y, 2) <= pow(cir.r, 2)) return true;
	if (pow((double)boxX1 - cir.p.x, 2) + pow((double)boxY2 - cir.p.y, 2) <= pow(cir.r, 2)) return true;
	if (pow((double)boxX2 - cir.p.x, 2) + pow((double)boxY1 - cir.p.y, 2) <= pow(cir.r, 2)) return true;
	if (pow((double)boxX2 - cir.p.x, 2) + pow((double)boxY2 - cir.p.y, 2) <= pow(cir.r, 2)) return true;
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
	player_->SetCamera(camera_);
	player_->Initialize();

	stage_ = new Stage();
	stage_->SetCamera(camera_);
	stage_->Initialize();
}

// �X�V����
void GameScene::Update() {
	// �v���C���[�X�V����
	player_->Update();

	// �J�����̍X�V
	camera_->Update();

	// �����蔻�菈��
	Collision();
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
			WallSlide(stage_->wallsCol_[i]);
		}
	}
}

void GameScene::WallSlide(Box2D& wall) {
	Line2D lines[4];
	lines[0] = { {wall.p.x + wall.rX, wall.p.y + wall.rY}, {wall.p.x + wall.rX, wall.p.y - wall.rY} };
	lines[1] = { {wall.p.x + wall.rX, wall.p.y - wall.rY}, {wall.p.x - wall.rX, wall.p.y - wall.rY} };
	lines[2] = { {wall.p.x - wall.rX, wall.p.y - wall.rY}, {wall.p.x - wall.rX, wall.p.y + wall.rY} };
	lines[3] = { {wall.p.x - wall.rX, wall.p.y + wall.rY}, {wall.p.x + wall.rX, wall.p.y + wall.rY} };

	float minDist = 1000.0f;// -> �ŒZ����

	bool isIntersection[4];// -> �������Ă��邩
	Vector2 intersection[4];// -> �����_
	Vector2 inte;// -> �ŒZ�����_
	bool flag = false;

	Line2D playerMoveLine = { player_->oldCol_.p, player_->col_.p + Vector2(player_->col_.p - player_->oldCol_.p).normalize() * 10.0f};
	for (size_t i = 0; i < 4; i++) {
		isIntersection[i] = calcSegmentIntersectPos(playerMoveLine, lines[i], &intersection[i]);
		if (isIntersection[i]) {
			if (Vector2(intersection[i] - player_->oldCol_.p).length() < minDist) {
				minDist = Vector2(intersection[i] - player_->oldCol_.p).length();
				inte = intersection[i];
			}
			flag = true;
		}
	}

	if (flag == false) {
		return;
	}

	Vector2 vec = player_->oldCol_.p - inte;
	Vector2 vecNor = vec;
	Vector2 vec1 = vecNor.normalize() * player_->col_.r;
	player_->col_.p = inte + vec1;
	player_->object_->position_.x = inte.x + vec1.x;
	player_->object_->position_.z = inte.y + vec1.y;
	//camera_->eye_.x = camera_->eye_.x + vec1.x;
	//camera_->eye_.z = camera_->eye_.z + vec1.y;
}