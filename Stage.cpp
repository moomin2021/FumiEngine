#include "Stage.h"

Stage::Stage() :
	// モデル
	wallM_(nullptr),// -> 壁

	// オブジェクト
	wallsObj_{},

	// 当たり判定用
	wallsCol2D_{}
{}

Stage::~Stage() {
	delete wallM_;
	delete doorM_;
	wallsObj_.clear();
	wallsCol2D_.clear();
}

void Stage::Initialize() {
	wallM_ = Model::CreateModel("wall");// -> 壁
	doorM_ = Model::CreateModel("door");// -> ドア

	int area[27][27] = {
	{1,2,1,2,1,2,1,2,1,2,1,2,1,0,1,2,1,2,1,2,1,2,1,2,1,2,1},
	{2,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,2},
	{1,0,0,0,1,2,1,2,1,2,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1},
	{2,0,0,0,2,0,0,0,0,0,0,0,2,0,2,0,2,0,0,0,0,0,0,0,2,0,2},
	{1,0,1,2,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1},
	{2,0,2,0,0,0,0,0,0,0,0,0,2,0,2,0,2,0,0,0,0,0,0,0,2,0,2},
	{1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,2,1,2,1,2,1,2,1,0,1},
	{2,0,2,0,0,0,0,0,0,0,0,0,2,0,2,0,2,0,0,0,0,0,0,0,2,0,2},
	{1,0,1,2,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1},
	{2,0,0,0,2,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,2,0,2},
	{1,0,0,0,1,2,1,2,1,2,1,2,1,3,1,0,1,0,0,0,0,0,0,0,1,0,1},
	{2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,2},
	{1,2,1,2,1,2,1,2,1,2,1,0,0,0,1,0,1,2,1,2,1,2,1,2,1,2,1},
	{0,0,0,0,0,0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,1,2,1,2,1,2,1,2,1,0,1,3,1,2,1,2,1,2,1,0,1,2,1,2,1},
	{2,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,2,0,2,0,0,0,2},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,1},
	{2,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,2,0,2,0,0,0,2},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,1},
	{2,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,2,0,2,0,0,0,2},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,1},
	{2,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,2,0,2,0,0,0,2},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,1},
	{2,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,2,0,2,0,0,0,2},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,2,1,0,1},
	{2,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,2},
	{1,2,1,2,1,2,1,2,1,2,1,2,1,0,1,2,1,2,1,2,1,2,1,2,1,2,1}};

	// コピー
	for (size_t i = 0; i < 27; i++) {
		for (size_t j = 0; j < 27; j++) {
			area_[i][j] = area[i][j];
		}
	}

	for (size_t i = 0; i < 27; i++) {
		for (size_t j = 0; j < 27; j++) {
			// 壁
			if (area_[i][j] == 2) {
				Object3D* wall = Object3D::CreateObject3D();
				wall->SetModel(wallM_);
				wall->SetCamera(camera_);
				wall->scale_ = { 10.0f, 10.0f, 10.0f };
				wall->position_ = { j * 5.0f, 5.0f, i * -5.0f};
				if (i % 2 == 0) wall->rotation_.y = 90.0f;
				wallsObj_.emplace_back(*wall);

				Line2D obj{};
				if (wall->rotation_.y >= 90.0f) {
					obj.sP.x = wall->position_.x - 5.0f;
					obj.sP.y = wall->position_.z;
					obj.eP.x = wall->position_.x + 5.0f;
					obj.eP.y = wall->position_.z;
				}
				else {
					obj.sP.x = wall->position_.x;
					obj.sP.y = wall->position_.z - 5.0f;
					obj.eP.x = wall->position_.x;
					obj.eP.y = wall->position_.z + 5.0f;
				}
				wallsCol2D_.emplace_back(obj);

				Board board{};
				if (wall->rotation_.y >= 90.0f) {
					board.p[0] = { wall->position_.x + 5.0f, wall->position_.y + 5.0f, wall->position_.z };
					board.p[1] = { wall->position_.x - 5.0f, wall->position_.y + 5.0f, wall->position_.z };
					board.p[2] = { wall->position_.x - 5.0f, wall->position_.y - 5.0f, wall->position_.z };
					board.p[3] = { wall->position_.x + 5.0f, wall->position_.y - 5.0f, wall->position_.z };
				}
				else {
					board.p[0] = { wall->position_.x, wall->position_.y + 5.0f, wall->position_.z + 5.0f };
					board.p[1] = { wall->position_.x, wall->position_.y + 5.0f, wall->position_.z - 5.0f };
					board.p[2] = { wall->position_.x, wall->position_.y - 5.0f, wall->position_.z - 5.0f };
					board.p[3] = { wall->position_.x, wall->position_.y - 5.0f, wall->position_.z + 5.0f };
				}
				wallsCol3D_.emplace_back(board);
				delete wall;
			}

			else if (area[i][j] == 3) {
				doors_.push_back(Door());
				doors_.push_back(Door());

				size_t i1 = doors_.size() - 2;
				size_t i2 = doors_.size() - 1;
				
				doors_[i1].Initialize();
				doors_[i2].Initialize();
				doors_[i1].SetCamera(camera_);
				doors_[i2].SetCamera(camera_);
				doors_[i1].SetModel(doorM_);
				doors_[i2].SetModel(doorM_);

				if (i % 2 == 0) {
					doors_[i1].object_.position_ = { j * 5.0f + 2.5f, 5.0f, i * -5.0f - 0.025f };
					doors_[i1].isRight_ = true;
					doors_[i2].object_.position_ = { j * 5.0f - 2.5f, 5.0f, i * -5.0f + 0.025f };
					doors_[i2].isRight_ = false;
				}
				else {
					doors_[i1].object_.position_ = { j * 5.0f - 0.025f, 5.0f, i * -5.0f + 2.5f };
					doors_[i1].isRight_ = true;
					doors_[i2].object_.position_ = { j * 5.0f + 0.025f, 5.0f, i * -5.0f - 2.5f };
					doors_[i2].isRight_ = false;
					doors_[i1].object_.rotation_.y = 90.0f;
					doors_[i2].object_.rotation_.y = 90.0f;
				}
			}
		}
	}
}

void Stage::Update()
{
	for (size_t i = 0; i < doors_.size(); i++) {
		doors_[i].Update();
	}
}

void Stage::Draw() {
	Object3D::PreDraw();
	for (size_t i = 0; i < wallsObj_.size(); i++) {
		wallsObj_[i].Draw();
	}

	for (size_t i = 0; i < doors_.size(); i++) {
		doors_[i].Draw();
	}
}