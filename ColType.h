#pragma once
#include "Vector2.h"

struct Line2D {
	Vector2 sP;// -> 開始点
	Vector2 eP;// -> 終了点

	// 開始点から終了点までのベクトルを取得
	Vector2 vec() { return eP - sP; }
};

struct Box2D {
	Vector2 p;// -> 中心点
	float rX, rY;// -> 半径
};

struct Circle2D {
	Vector2 p;// -> 中心点
	float r;// -> 半径
};