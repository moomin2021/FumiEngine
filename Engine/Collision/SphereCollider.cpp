#include "SphereCollider.h"

void SphereCollider::Update()
{
	// ワールド行列からの座標を抽出
	const Matrix4& matWorld = object3d_->GetMatWorld();

	// 球のメンバ変数を更新
	Sphere::center = Vector3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]) + offset_;
	Sphere::radius = radius_;
}