#pragma once
#include "Vector3.h"

class Wall {
	// メンバ変数
public:
	Vector3 pos_;// -> 座標

private:

	// メンバ関数
public:
	// コンストラクタ
	Wall();

	// デストラクタ
	~Wall();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:

};