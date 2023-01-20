#pragma once
#include "Object3D.h"
#include "Model.h"
#include "Camera.h"
#include "ColType.h"
#include "Door.h"
#include <array>
#include <memory>
#include "Wall.h"
#include <vector>

struct Area {
	int area[27][27];
};

class Stage {
	// メンバ変数
public:
	//std::vector<std::unique_ptr<Object3D>> wallsObj_;// 壁
	//std::array<std::unique_ptr<Object3D>, 2000> wallsObj_;
	//std::vector<Line2D> wallsCol2D_;// 2D用当たり判定
	//std::vector<Board> wallsCol3D_;// 3D用当たり判定
	size_t wallCount_;
	std::array<std::unique_ptr<Wall>, 1400> walls_;

	size_t doorCount_;
	std::array<std::unique_ptr<Door>, 300> doors_;

	std::vector<std::unique_ptr<Object3D>> floors_;

	std::vector<std::unique_ptr<Object3D>> ceiling_;

	std::unique_ptr<Object3D> magatama_;
	Circle2D magatamaCol_;

	//std::array<Area, 4> area_;

	std::array<Area, 1> areaStart_;
	std::array<Area, 2> area4Way_;
	std::array<Area, 3> area3Way_;
	std::array<Area, 4> area2Way_;

private:
	//int area_[27][27];

	Model* wallM_;
	Model* doorM_;// -> ドアモデル
	Model* floorM_;
	Model* ceilingM_;
	Model* magatamaM_;

	Camera* camera_;

	// メンバ関数
public:
	// コンストラクタ
	Stage();

	// デストラクタ
	~Stage();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; }

	void LoadStage(std::string path, Area& copyArea);

	void GeneArea(int areaNum, Vector2 areaPos);

private:

};