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

// コンストラクタ
GameScene::GameScene() :
	key_(nullptr),// -> キーボード入力

	camera_(nullptr),// -> カメラ

	player_(nullptr),// -> プレイヤー

	stage_(nullptr),// -> ステージ

	// モデル
	blackFloorM_(nullptr),// -> 黒色の床
	whiteFloorM_(nullptr),// -> 白色の床
	wallM_(nullptr),// -> 壁

	// オブジェクト
	floor_{}// -> 床
{

}

// デストラクタ
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

// 初期化処理
void GameScene::Initialize() {
	// キーボード入力
	key_ = Key::GetInstance();

	// モデル
	blackFloorM_ = Model::CreateModel("blackFloor");// -> 黒色の床
	whiteFloorM_ = Model::CreateModel("whiteFloor");// -> 白色の床
	wallM_ = Model::CreateModel("wall");// -> 壁

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { -10.0f, 5.0f, 0.0f };
	camera_->target_ = { 0.0f, 5.0f, 10.0f };

	// オブジェクト
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

	// プレイヤー初期化処理
	player_ = new Player();
	player_->Initialize();
	player_->SetCamera(camera_);

	stage_ = new Stage();
	stage_->SetCamera(camera_);
	stage_->Initialize();
}

// 更新処理
void GameScene::Update() {
	// プレイヤー更新処理
	player_->Update();

	// 当たり判定処理
	Collision();

	// カメラの更新
	camera_->Update();
}

// 描画処理
void GameScene::Draw() {

	// モデル描画前処理
	Object3D::PreDraw();

	// プレイヤーモデル描画
	for (size_t i = 0; i < maxFloor_; i++) floor_[i]->Draw();

	//for (size_t i = 0; i < maxWall_; i++) wallObj_[i]->Draw();

	// プレイヤー描画処理
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