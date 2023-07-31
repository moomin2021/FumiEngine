#include "SphereCollider.h"
#include "Matrix4.h"

SphereCollider::SphereCollider(float3 offset, float radius) :
	offset_(offset), radius_(radius)
{
	// 形状タイプを球に設定
	shapeType_ = SHAPE_SPHERE;
}

void SphereCollider::Update()
{
	// 衝突フラグを初期化
	isHit_ = false;
	reject_ = { 0.0f, 0.0f, 0.0f };

	// オブジェクト3Dが紐づけられていたら
	if (object_) {
		// ワールド行列からワールド座標を抽出
		const Matrix4& matWorld = object_->GetMatWorld();
		Sphere::center = Vector3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]) + Vector3(offset_);
	}

	else {
		Sphere::center = offset_;
	}

	Sphere::radius = radius_;
}