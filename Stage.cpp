#include "Stage.h"

Stage::Stage() :
	// モデル
	wallM_(nullptr),// -> 壁

	// オブジェクト
	wallsObj_{},

	// 当たり判定用
	wallsCol_{}
{}

Stage::~Stage() {
	delete wallM_;
	wallsObj_.clear();
	wallsCol_.clear();
}

void Stage::Initialize() {
	wallM_ = Model::CreateModel("wall");// -> 壁

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
	{1,0,0,0,1,2,1,2,1,2,1,2,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1},
	{2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,2},
	{1,2,1,2,1,2,1,2,1,2,1,0,0,0,1,0,1,2,1,2,1,2,1,2,1,2,1},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,1,2,1,2,1,2,1,2,1,0,1,0,1,2,1,2,1,2,1,0,1,2,1,2,1},
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
				wallsCol_.emplace_back(obj);
				delete wall;
			}
		}
	}
}

void Stage::Draw() {
	Object3D::PreDraw();
	for (size_t i = 0; i < wallsObj_.size(); i++) {
		wallsObj_[i].Draw();
	}
}