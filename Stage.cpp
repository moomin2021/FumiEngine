#include "Stage.h"
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>

using namespace std;

void Shuffle(int array[], int size) {
	random_device device;
	mt19937 mt(device());
	uniform_int_distribution<> rand(0, 100);

	for (size_t i = 0; i < size; i++) {
		int j = rand(mt) % size;
		int t = array[i];
		array[i] = array[j];
		array[j] = t;
	}
}

Stage::Stage() :
	// ÉÇÉfÉã
	wallM_(nullptr)// -> ï«
{}

Stage::~Stage() {
	delete wallM_;
	delete doorM_;
	delete floorM_;
	delete ceilingM_;
	delete magatamaM_;
}

void Stage::Initialize() {
	wallCount_ = 0;
	doorCount_ = 0;

	wallM_ = Model::CreateModel("wall");// -> ï«
	doorM_ = Model::CreateModel("door");// -> ÉhÉA
	floorM_ = Model::CreateModel("floor");// -> è∞
	ceilingM_ = Model::CreateModel("ceiling");// -> ìVà‰
	magatamaM_ = Model::CreateModel("magatama");

	// CSVì«Ç›çûÇ›
	LoadStage("csv/areaStart.csv", areaStart_.at(0));
	LoadStage("csv/area4Way.csv", area4Way_.at(0));
	LoadStage("csv/area4Way.csv", area4Way_.at(1));
	LoadStage("csv/area3Way_1.csv", area3Way_.at(0));
	LoadStage("csv/area3Way_1.csv", area3Way_.at(1));
	LoadStage("csv/area3Way_1.csv", area3Way_.at(2));
	LoadStage("csv/area2Way_1.csv", area2Way_.at(0));
	LoadStage("csv/area2Way_2.csv", area2Way_.at(1));
	LoadStage("csv/area2Way_1.csv", area2Way_.at(2));
	LoadStage("csv/area2Way_1.csv", area2Way_.at(3));

	// ê∂ê¨èáÇåàÇﬂÇÈ
	int way3[] = { 0, 1, 2 };
	int way2[] = { 0, 1, 2, 3 };

	Shuffle(way3, 3);
	Shuffle(way2, 4);

	for (size_t i = 0; i < 3; i++) {
		int area[27][27];
		copy(&area3Way_.at(way3[i]).area[0][0], &area3Way_.at(way3[i]).area[26][26], &area[0][0]);

		// 270ìxâÒì]
		if (i == 1) {
			for (size_t x = 0; x < 27; x++) {
				for (size_t y = 0; y < 27; y++) {
					area3Way_.at(way3[i]).area[x][y] = area[y][27 - 1 - x];
				}
			}
		}

		// 90ìxâÒì]
		else if (i == 2) {
			for (size_t x = 0; x < 27; x++) {
				for (size_t y = 0; y < 27; y++) {
					area3Way_.at(way3[i]).area[x][y] = area[27 - 1 - y][x];
				}
			}
		}
	}

	for (size_t i = 0; i < 4; i++) {
		int area[27][27];
		copy(&area2Way_.at(way2[i]).area[0][0], &area2Way_.at(way2[i]).area[26][26], &area[0][0]);

		// 90ìxâÒì]
		if (i == 1) {
			for (size_t x = 0; x < 27; x++) {
				for (size_t y = 0; y < 27; y++) {
					area2Way_.at(way2[i]).area[x][y] = area[27 - 1 - y][x];
				}
			}
		}

		// 270ìxâÒì]
		else if (i == 2) {
			for (size_t x = 0; x < 27; x++) {
				for (size_t y = 0; y < 27; y++) {
					area2Way_.at(way2[i]).area[x][y] = area[y][27 - 1 - x];
				}
			}
		}

		// 180ìxâÒì]
		else if (i == 3) {
			for (size_t x = 0; x < 27; x++) {
				for (size_t y = 0; y < 27; y++) {
					area2Way_.at(way2[i]).area[x][y] = area[27 - 1 - x][27 - 1 - y];
				}
			}
		}
	}

	//int stage_[4][3] = {
	//	{20, 30, 21},
	//	{31, 40, 32},
	//	{22, 41, 23},
	//	{00, 10, 00}
	//};

	//int stage_[4][3] = {
	//	{20, 00, 00},
	//	{00, 00, 00},
	//	{00, 00, 00},
	//	{00, 00, 00}
	//};

	int stage_[4][3] = {
		{way2[0] + 20,way3[0] + 30, way2[1] + 20},
		{way3[1] + 30, 40, way3[2] + 30},
		{way2[2] + 20, 40, way2[3] + 20},
		{00, 10, 00}
	};

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 3; j++) {
			GeneArea(stage_[i][j], { j * 130.0f, i * 130.0f });
		}
	}

	for (size_t i = 0; i < 52; i++) {
		for (size_t j = 0; j < 39; j++) {
			floors_.push_back(std::make_unique<Object3D>());
			size_t index = floors_.size() - 1;
			floors_[index]->position_ = { j * 10.0f + 5.0f, 0.0f, i * -10.0f - 5.0f };
			floors_[index]->scale_ = { 10.0f, 10.0f, 10.0f };
			floors_[index]->SetCamera(camera_);
			floors_[index]->SetModel(floorM_);
		}
	}

	for (size_t i = 0; i < 52; i++) {
		for (size_t j = 0; j < 39; j++) {
			ceiling_.push_back(std::make_unique<Object3D>());
			size_t index = ceiling_.size() - 1;
			ceiling_[index]->position_ = { j * 10.0f + 5.0f, 10.0f, i * -10.0f - 5.0f };
			ceiling_[index]->scale_ = { 10.0f, 10.0f, 10.0f };
			ceiling_[index]->SetCamera(camera_);
			ceiling_[index]->SetModel(ceilingM_);
		}
	}
}

void Stage::Update()
{
	for (size_t i = 0; i < doorCount_; i++) {
		doors_[i]->Update();
	}
}

void Stage::Draw() {
	magatama_->Draw();
}

void Stage::LoadStage(string path, Area& copyArea) {
	Area area;

	ifstream ifs(path);
	string line;
	string tmp;

	size_t countY = 0;

	while (getline(ifs, line)) {
		istringstream line_stream(line);

		size_t countX = 0;

		while (getline(line_stream, tmp, ',')) {
			area.area[countY][countX] = stoi(tmp);

			countX++;
		}

		countY++;
	}

	copy(&area.area[0][0], &area.area[26][26], &copyArea.area[0][0]);
}

void Stage::GeneArea(int areaNum, Vector2 areaPos)
{
	int area[27][27];

	if (areaNum >= 10 && areaNum < 20) {
		copy(&areaStart_.at(areaNum - 10).area[0][0], &areaStart_.at(areaNum - 10).area[26][26], &area[0][0]);
	}
	else if (areaNum >= 20 && areaNum < 30) {
		copy(&area2Way_.at(areaNum - 20).area[0][0], &area2Way_.at(areaNum - 20).area[26][26], &area[0][0]);
	}
	else if (areaNum >= 30 && areaNum < 40) {
		copy(&area3Way_.at(areaNum - 30).area[0][0], &area3Way_.at(areaNum - 30).area[26][26], &area[0][0]);
	}
	else if (areaNum >= 40) {
		copy(&area4Way_.at(areaNum - 40).area[0][0], &area4Way_.at(areaNum - 40).area[26][26], &area[0][0]);
	}

	for (size_t i = 0; i < 27; i++) {
		for (size_t j = 0; j < 27; j++) {
			// ï«
			if (area[i][j] == 2) {
				walls_.at(wallCount_) = std::make_unique<Wall>();
				walls_[wallCount_]->object_->SetModel(wallM_);
				walls_[wallCount_]->object_->SetCamera(camera_);
				walls_[wallCount_]->object_->scale_ = { 10.0f, 10.0f, 10.0f };
				walls_[wallCount_]->object_->position_ = { j * 5.0f + areaPos.x, 5.0f, i * -5.0f - areaPos.y };
				if (i % 2 == 0) walls_[wallCount_]->object_->rotation_.y = 0.0f;
				else walls_[wallCount_]->object_->rotation_.y = 90.0f;

				// 2DÇÃìñÇΩÇËîªíË
				if (walls_[wallCount_]->object_->rotation_.y >= 90.0f) {
					walls_[wallCount_]->col2D_.sP.x = walls_[wallCount_]->object_->position_.x;
					walls_[wallCount_]->col2D_.sP.y = walls_[wallCount_]->object_->position_.z - 5.0f;
					walls_[wallCount_]->col2D_.eP.x = walls_[wallCount_]->object_->position_.x;
					walls_[wallCount_]->col2D_.eP.y = walls_[wallCount_]->object_->position_.z + 5.0f;
				}
				else {
					walls_[wallCount_]->col2D_.sP.x = walls_[wallCount_]->object_->position_.x - 5.0f;
					walls_[wallCount_]->col2D_.sP.y = walls_[wallCount_]->object_->position_.z;
					walls_[wallCount_]->col2D_.eP.x = walls_[wallCount_]->object_->position_.x + 5.0f;
					walls_[wallCount_]->col2D_.eP.y = walls_[wallCount_]->object_->position_.z;
				}

				// 3DÇÃìñÇΩÇËîªíË
				if (walls_[wallCount_]->object_->rotation_.y >= 90.0f) {
					walls_[wallCount_]->col3D_.p[0] = { walls_[wallCount_]->object_->position_.x, walls_[wallCount_]->object_->position_.y + 5.0f, walls_[wallCount_]->object_->position_.z + 5.0f };
					walls_[wallCount_]->col3D_.p[1] = { walls_[wallCount_]->object_->position_.x, walls_[wallCount_]->object_->position_.y + 5.0f, walls_[wallCount_]->object_->position_.z - 5.0f };
					walls_[wallCount_]->col3D_.p[2] = { walls_[wallCount_]->object_->position_.x, walls_[wallCount_]->object_->position_.y - 5.0f, walls_[wallCount_]->object_->position_.z - 5.0f };
					walls_[wallCount_]->col3D_.p[3] = { walls_[wallCount_]->object_->position_.x, walls_[wallCount_]->object_->position_.y - 5.0f, walls_[wallCount_]->object_->position_.z + 5.0f };
				}
				else {
					walls_[wallCount_]->col3D_.p[0] = { walls_[wallCount_]->object_->position_.x + 5.0f, walls_[wallCount_]->object_->position_.y + 5.0f, walls_[wallCount_]->object_->position_.z };
					walls_[wallCount_]->col3D_.p[1] = { walls_[wallCount_]->object_->position_.x - 5.0f, walls_[wallCount_]->object_->position_.y + 5.0f, walls_[wallCount_]->object_->position_.z };
					walls_[wallCount_]->col3D_.p[2] = { walls_[wallCount_]->object_->position_.x - 5.0f, walls_[wallCount_]->object_->position_.y - 5.0f, walls_[wallCount_]->object_->position_.z };
					walls_[wallCount_]->col3D_.p[3] = { walls_[wallCount_]->object_->position_.x + 5.0f, walls_[wallCount_]->object_->position_.y - 5.0f, walls_[wallCount_]->object_->position_.z };
				}

				wallCount_++;
			}

			else if (area[i][j] == 3) {
				doors_.at(doorCount_) = std::make_unique<Door>();
				doors_.at(doorCount_ + 1) = std::make_unique<Door>();

				size_t i1 = doorCount_;
				size_t i2 = doorCount_ + 1;

				doors_[i1]->Initialize();
				doors_[i2]->Initialize();
				doors_[i1]->object_->SetCamera(camera_);
				doors_[i2]->object_->SetCamera(camera_);
				doors_[i1]->object_->SetModel(doorM_);
				doors_[i2]->object_->SetModel(doorM_);

				if (i % 2 == 0) {
					doors_[i1]->object_->position_ = { j * 5.0f + 2.5f + areaPos.x, 5.0f, i * -5.0f - 0.025f - areaPos.y };
					doors_[i1]->isRight_ = true;
					doors_[i2]->object_->position_ = { j * 5.0f - 2.5f + areaPos.x, 5.0f, i * -5.0f + 0.025f - areaPos.y };
					doors_[i2]->isRight_ = false;
				}
				else {
					doors_[i1]->object_->position_ = { j * 5.0f - 0.025f + areaPos.x, 5.0f, i * -5.0f + 2.5f - areaPos.y };
					doors_[i1]->isRight_ = true;
					doors_[i2]->object_->position_ = { j * 5.0f + 0.025f + areaPos.x, 5.0f, i * -5.0f - 2.5f - areaPos.y };
					doors_[i2]->isRight_ = false;
					doors_[i1]->object_->rotation_.y = 90.0f;
					doors_[i2]->object_->rotation_.y = 90.0f;
				}

				doorCount_ += 2;
			}

			else if (area[i][j] == 4) {
				magatama_ = std::make_unique<Object3D>();
				magatama_->scale_ = { 5.0f, 5.0f, 5.0f };
				magatama_->position_ = { j * 5.0f + areaPos.x, 5.0f, i * -5.0f - areaPos.y };
				magatama_->SetCamera(camera_);
				magatama_->SetModel(magatamaM_);

				magatamaCol_.p.x = magatama_->position_.x;
				magatamaCol_.p.y = magatama_->position_.z;
				magatamaCol_.r = 1.0f;
			}
		}
	}
}
