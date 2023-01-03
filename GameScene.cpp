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

	// プレイヤー描画処理
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