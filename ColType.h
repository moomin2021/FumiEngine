#pragma once
#include "Vector2.h"
#include "Vector3.h"

struct Line2D {
	Vector2 sP;// -> �J�n�_
	Vector2 eP;// -> �I���_

	// �J�n�_����I���_�܂ł̃x�N�g�����擾
	Vector2 vec() { return eP - sP; }
};

struct Box2D {
	Vector2 p;// -> ���S�_
	float rX, rY;// -> ���a
};

struct Circle2D {
	Vector2 p;// -> ���S�_
	float r;// -> ���a
};

struct Line3D {
	Vector3 sP;// -> �J�n�_
	Vector3 eP;// -> �I���_
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