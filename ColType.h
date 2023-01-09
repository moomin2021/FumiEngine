#pragma once
#include "Vector2.h"

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