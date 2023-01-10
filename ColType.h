#pragma once
#include "Vector2.h"
#include "Vector3.h"

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

struct Line3D {
	Vector3 sP;// -> 開始点
	Vector3 eP;// -> 終了点
};

struct Triangle3D {
	Vector3 p0, p1, p2;

	Vector3 normal() {
		Vector3 p02p1 = p1 - p0;
		Vector3 p02p2 = p2 - p0;

		Vector3 normal = p02p1.cross(p02p2);
		return normal.normalize();
	}
};

struct Board {
	Vector3 p[4];

	Vector3 normal() {
		Vector3 p02p1 = p[1] - p[0];
		Vector3 p02p2 = p[2] - p[0];

		Vector3 normal = p02p1.cross(p02p2);
		return normal.normalize();
	}
};