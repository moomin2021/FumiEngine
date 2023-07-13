#include "RayCollider.h"

RayCollider::RayCollider(float3 offset, Vector3 dir) :
	offset_(offset)
{
	shapeType_ = SHAPE_RAY;

	Ray::dir = dir;
}

void RayCollider::Update()
{
	// 衝突フラグを初期化
	isHit_ = false;

	// オブジェクト3Dが紐づけられていたら
	if (object_) {
		// ワールド行列からワールド座標を抽出
		const Matrix4& matWorld = object_->GetMatWorld();
		Ray::start = Vector3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]) + Vector3(offset_);
	}

	else {
		Ray::start = offset_;
	}
}