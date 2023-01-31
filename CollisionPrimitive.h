#pragma once
#include <DirectXMath.h>

struct Sphere {
	// ���S���W
	DirectX::XMVECTOR center = { 0.0f, 0.0f, 0.0f, 1.0f };

	// ���a
	float radius = 1.0f;
};

struct Plane {
	// �@���x�N�g��
	DirectX::XMVECTOR normal = { 0.0f, 1.0f, 0.0f, 0.0f };

	// ���_(0.0f, 0.0f, 0.0f)����̋���
	float distance = 0.0f;
};

class Triangle {
public:
	// ���_���W3��
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;

	// �@���x�N�g��
	DirectX::XMVECTOR normal;
};