#include "GameScene.h"
#include "FumiEngine.h"

bool calcSegmentIntersectPos(Line2D line1, Line2D line2, Vector2* intersect) {
	Vector2 vec1 = line1.eP - line1.sP;
	Vector2 vec2 = line2.eP - line2.sP;

	float cV2V1 = vec2.cross(vec1);
	if (cV2V1 == 0.0f) return false;// -> 平行なので[false]を返す

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

bool CirLineCol(Circle2D cir, Line2D line, float& minDist) {
	// 点から線分に向かって垂直に線を引いた時のベクトルを求める
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
	player_->SetCamera(camera_);
	player_->Initialize();

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
	for (size_t i = 0; i < stage_->wallsCol_.size(); i++) {
		float dist = 0.0f;
		if (CirLineCol(player_->col_, stage_->wallsCol_[i], dist)) {
			WallSlide(stage_->wallsCol_[i]);
		}
	}
}

void GameScene::WallSlide(Line2D& wall) {
	// 点から線分に向かって垂直に線を引いた時のベクトルを求める
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
	player_->object_->position_.x = resultPos.x;
	player_->object_->position_.z = resultPos.y;
	camera_->eye_.x = resultPos.x;
	camera_->eye_.z = resultPos.y;

	player_->Target();
}