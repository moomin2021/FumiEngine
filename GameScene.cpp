#include "GameScene.h"
#include "FumiEngine.h"
#include "SceneManager.h"

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

bool CircleCol(Circle2D cir1, Circle2D cir2) {
	float a = cir1.p.x - cir2.p.x;
	float b = cir1.p.y - cir2.p.y;
	float c = sqrt(a * a + b * b);
	if (c <= cir1.r + cir2.r) return true;
	return false;
}

bool RayBoardCol(Line3D line, Board board, float &minDist) {
	Vector3 p0 = board.p[0];// -> 平面の適当な座標

	Vector3 p02sP = line.sP - p0;
	Vector3 p02eP = line.eP - p0;

	Vector3 boardN = board.normal();

	float d1 = boardN.dot(p02sP);
	float d2 = boardN.dot(p02eP);

	if (d1 * d2 > 0) return false;

	float m = fabsf(d1);
	float n = fabsf(d2);

	// 平面と線分の交点
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

	stage_(nullptr)// -> ステージ
{

}

// デストラクタ
GameScene::~GameScene() {
	delete camera_;
	delete player_;
	delete stage_;
	delete openOrClose_;
}

// 初期化処理
void GameScene::Initialize() {
	// キーボード入力
	key_ = Key::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 195.0f,5.0f, -480.0f };
	camera_->target_ = { 0.0f, 5.0f, 10.0f };

	// プレイヤー初期化処理
	player_ = new Player();
	player_->SetCamera(camera_);
	player_->Initialize();

	stage_ = new Stage();
	stage_->SetCamera(camera_);
	stage_->Initialize();

	openOrClose_ = new Sprite();
	openOrClose_->position.x = WinAPI::GetWidth() / 2.0f - 75.0f;
	openOrClose_->position.y = WinAPI::GetHeight() / 2.0f + 100.0f;
	openOrClose_->scale = { 1.5f, 1.5f };
	openOrClose_->Update();

	openOrCloseH_ = LoadTexture("Resources/openOrClose1.png");

	hintText_ = std::make_unique<Sprite>();
	hintText_->scale = { 19.20f, 10.80f };
	hintText_->Update();

	hintTextH_ = LoadTexture("Resources/hintText.png");
}

// 更新処理
void GameScene::Update() {
	// プレイヤー更新処理
	player_->Update();

	stage_->Update();

	// 当たり判定処理
	Collision();

	// カメラの更新
	camera_->Update();

	if (CircleCol(player_->col_, stage_->magatamaCol_)) {
		SceneManager::ChangeScene(SCENE::CLEAR);
	}
}

// 描画処理
void GameScene::Draw() {

	// モデル描画前処理
	Object3D::PreDraw();
	for (size_t i = 0; i < stage_->wallCount_;i++) {
		Vector3 len;
		len.x = stage_->walls_[i]->object_->position_.x - camera_->eye_.x;
		len.y = stage_->walls_[i]->object_->position_.y - camera_->eye_.y;
		len.z = stage_->walls_[i]->object_->position_.z - camera_->eye_.z;
		if (len.length() < 100.0f) {
			stage_->walls_[i]->Draw();
		}
	}

	for (size_t i = 0; i < stage_->doorCount_; i++) {
		Vector3 len;
		len.x = stage_->doors_[i]->object_->position_.x - camera_->eye_.x;
		len.y = stage_->doors_[i]->object_->position_.y - camera_->eye_.y;
		len.z = stage_->doors_[i]->object_->position_.z - camera_->eye_.z;
		if (len.length() < 100.0f) {
			stage_->doors_[i]->Draw();
		}
	}

	for (size_t i = 0; i < stage_->floors_.size() - 1; i++) {
		Vector3 len;
		len.x = stage_->floors_[i]->position_.x - camera_->eye_.x;
		len.y = stage_->floors_[i]->position_.y - camera_->eye_.y;
		len.z = stage_->floors_[i]->position_.z - camera_->eye_.z;
		if (len.length() < 100.0f) {
			stage_->floors_[i]->Draw();
		}
	}

	for (size_t i = 0; i < stage_->ceiling_.size() - 1; i++) {
		Vector3 len;
		len.x = stage_->ceiling_[i]->position_.x - camera_->eye_.x;
		len.y = stage_->ceiling_[i]->position_.y - camera_->eye_.y;
		len.z = stage_->ceiling_[i]->position_.z - camera_->eye_.z;
		if (len.length() < 100.0f) {
			stage_->ceiling_[i]->Draw();
		}
	}
	stage_->Draw();

	// プレイヤー描画処理
	player_->Draw();

	Sprite::PreDraw();
	if (isText_) openOrClose_->Draw(openOrCloseH_);
	hintText_->Draw(hintTextH_);

}

void GameScene::Collision() {
	// 壁とプレイヤーの当たり判定
	WallCol();

	// ドアとプレイヤーの視線の当たり判定
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
		for (size_t i = 0; i < stage_->wallCount_; i++) {
			float dist = 0.0f;
			if (CirLineCol(player_->col_, stage_->walls_[i]->col2D_, dist)) {
				if (dist < minDist) {
					index = i;
					minDist = dist;
					num = 0;
				}
				isCol_ = true;
			}
		}

		for (size_t i = 0; i < stage_->doorCount_; i++) {
			float dist = 0.0f;
			if (CirLineCol(player_->col_, stage_->doors_[i]->col2D_, dist)) {
				if (dist < minDist) {
					index = i;
					minDist = dist;
					num = 1;
				}
				isCol_ = true;
			}
		}

		if (isCol_) {
			if (num == 0) WallSlide(stage_->walls_[index]->col2D_);
			if (num == 1) WallSlide(stage_->doors_[index]->col2D_);
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

	for (size_t i = 0; i < stage_->wallCount_; i++) {
		float dist = 0.0f;
		if (RayBoardCol(player_->ray_, stage_->walls_[i]->col3D_, dist)) {
			if (dist < minDist) {
				index = i;
				minDist = dist;
				num = 0;
				isCol = false;
			}
		}
	}

	for (size_t i = 0; i < stage_->doorCount_; i++) {
		float dist = 0.0f;
		if (RayBoardCol(player_->ray_, stage_->doors_[i]->col3D_, dist)) {
			if (dist < minDist) {
				index = i;
				minDist = dist;
				num = 1;
				isCol = true;
			}
		}
	}

	if (num == 1) {
		if (stage_->doors_[index]->isMove_ == false) {
			if (key_->TriggerKey(DIK_F)) {
				stage_->doors_[index]->isMove_ = true;

				Vector3 pos{};
				pos.x = stage_->doors_[index]->object_->position_.x;
				pos.y = stage_->doors_[index]->object_->position_.y;
				pos.z = stage_->doors_[index]->object_->position_.z;

				stage_->doors_[index]->oldPos_ = pos;
			}

			isText_ = isCol;
		}
	}
}