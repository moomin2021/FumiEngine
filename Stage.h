#pragma once
#include "Object3D.h"
#include "Model.h"
#include <vector>
#include "Camera.h"
#include "Vector2.h"

// 長方形データ
struct RectAngle {
	float x, y;
	float rX, rY;
};

class Stage {
	// メンバ変数
public:
	std::vector<Object3D> wallsObj_;// 壁
	std::vector<RectAngle> wallsCol_;// 壁当たり判定
private:
	int area_[27][27];

	Model* wallM_;

	Camera* camera_;

	// メンバ関数
public:
	// コンストラクタ
	Stage();

	// デストラクタ
	~Stage();

	// 初期化処理
	void Initialize();

	// 描画処理
	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; }

private:

};