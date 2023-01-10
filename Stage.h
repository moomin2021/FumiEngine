#pragma once
#include "Object3D.h"
#include "Model.h"
#include <vector>
#include "Camera.h"
#include "ColType.h"
#include "Door.h"

class Stage {
	// メンバ変数
public:
	std::vector<Object3D> wallsObj_;// 壁
	std::vector<Line2D> wallsCol2D_;// 2D用当たり判定
	std::vector<Board> wallsCol3D_;// 3D用当たり判定

	std::vector<Door> doors_;

	//std::vector<Object3D> doorsObj_;
	//std::vector<Line2D> doorsCol2D_;
	//std::vector<Door> doorsCol3D_;

	//std::vector<Door> doors_;
private:
	int area_[27][27];

	Model* wallM_;
	Model* doorM_;// -> ドアモデル

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

private:

};