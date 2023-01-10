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

bool RayBoardCol(Line3D line, Board board, float &minDist) {
	Vector3 p0 = board.p[0];// -> ���ʂ̓K���ȍ��W

	Vector3 p02sP = line.sP - p0;
	Vector3 p02eP = line.eP - p0;

	Vector3 boardN = board.normal();

	float d1 = boardN.dot(p02sP);
	float d2 = boardN.dot(p02eP);

	if (d1 * d2 > 0) return false;

	float m = fabsf(d1);
	float n = fabsf(d2);

	// ���ʂƐ����̌�_
	Vector3 ip = (line.sP * n + line.eP * m) / (m + n);

	minDist = (ip - line.sP).length();

	for (size_t i = 0; i < 4; i++) {
		Vector3 side = board.p[(i + 1) % 4] - board.p[i];
		Vector3 p2ip = ip - board.p[i];
		Vector3 c = side.cross(p2ip);
		float d = boardN.dot(c);
		if (d < 0) return false;
	}

	return true;
}

bool CirLineCol(Circle2D cir, Line2D line, float& minDist) {
	// �_��������Ɍ������Đ����ɐ������������̃x�N�g�������߂�
	Vector2 vec;
	float dot;
	{
		Vector2 sP2CirPVec = cir.p - line.sP;
		dot = sP2CirPVec.dot(line.vec().normalize());
		vec = (line.sP + line.vec().normalize() * dot) - cir.p;
		minDist = vec.length();
	}

	float t = dot / line.vec().length();
	if (t < 0.0f) {
		minDist = (line.sP - cir.p).length();
	}

	if (t > 1.0f) {
		minDist = (line.eP - cir.p).length();
	}

	if (cir.r > minDist) return true;
	return false;
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
	delete openOrClose_;
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

	openOrClose_ = new Sprite();
	openOrClose_->position.x = WinAPI::GetWidth() / 2.0f - 75.0f;
	openOrClose_->position.y = WinAPI::GetHeight() / 2.0f + 100.0f;
	openOrClose_->scale = { 1.5f, 0.8f };
	openOrClose_->Update();

	openOrCloseH_ = LoadTexture("Resources/openOrClose.png");
}

// �X�V����
void GameScene::Update() {
	// �v���C���[�X�V����
	player_->Update();

	stage_->Update();

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

	stage_->Draw();

	// �v���C���[�`�揈��
	player_->Draw();

	Sprite::PreDraw();
	if (isText_) openOrClose_->Draw(openOrCloseH_);
}

void GameScene::Collision() {
	// �ǂƃv���C���[�̓����蔻��
	WallCol();

	// �h�A�ƃv���C���[�̎����̓����蔻��
	DoorCol();
}

void GameScene::WallCol()
{
	bool isCol_ = true;

	while (isCol_) {
		isCol_ = false;
		float minDist = 1000.0f;
		size_t num = 0;
		size_t index = 0;
		for (size_t i = 0; i < stage_->wallsCol2D_.size(); i++) {
			float dist = 0.0f;
			if (CirLineCol(player_->col_, stage_->wallsCol2D_[i], dist)) {
				if (dist < minDist) {
					index = i;
					minDist = dist;
					num = 0;
				}
				isCol_ = true;
			}
		}

		for (size_t i = 0; i < stage_->doors_.size(); i++) {
			float dist = 0.0f;
			if (CirLineCol(player_->col_, stage_->doors_[i].col2D_, dist)) {
				if (dist < minDist) {
					index = i;
					minDist = dist;
					num = 1;
				}
				isCol_ = true;
			}
		}

		if (isCol_) {
			if (num == 0) WallSlide(stage_->wallsCol2D_[index]);
			if (num == 1) WallSlide(stage_->doors_[index].col2D_);
		}
	}
}

void GameScene::WallSlide(Line2D& wall) {
	// �_��������Ɍ������Đ����ɐ������������̃x�N�g�������߂�
	Vector2 resultPos;
	Vector2 vec;
	float dot;
	{
		Vector2 sP2CirPVec = player_->col_.p - wall.sP;
		dot = sP2CirPVec.dot(wall.vec().normalize());
		resultPos = wall.sP + wall.vec().normalize() * dot;
		vec = player_->col_.p - (wall.sP + wall.vec().normalize() * dot);
		resultPos += vec.normalize() * (player_->col_.r);
	}

	player_->col_.p = resultPos;
	camera_->eye_.x = resultPos.x;
	camera_->eye_.z = resultPos.y;

	player_->Target();
}

void GameScene::DoorCol() {

	size_t num = 0;
	size_t index = 0;
	bool isCol = false;
	float minDist = 1000.0f;
	isText_ = false;

	for (size_t i = 0; i < stage_->wallsCol3D_.size(); i++) {
		float dist = 0.0f;
		if (RayBoardCol(player_->ray_, stage_->wallsCol3D_[i], dist)) {
			if (dist < minDist) {
				index = i;
				minDist = dist;
				num = 0;
				isCol = false;
			}
		}
	}

	for (size_t i = 0; i < stage_->doors_.size(); i++) {
		float dist = 0.0f;
		if (RayBoardCol(player_->ray_, stage_->doors_[i].col3D_, dist)) {
			if (dist < minDist) {
				index = i;
				minDist = dist;
				num = 1;
				isCol = true;
			}
		}
	}

	if (num == 1) {
		if (stage_->doors_[index].isMove_ == false) {
			if (key_->TriggerKey(DIK_F)) {
				stage_->doors_[index].isMove_ = true;

				Vector3 pos{};
				pos.x = stage_->doors_[index].object_.position_.x;
				pos.y = stage_->doors_[index].object_.position_.y;
				pos.z = stage_->doors_[index].object_.position_.z;

				stage_->doors_[index].oldPos_ = pos;
			}

			isText_ = isCol;
		}
	}
}