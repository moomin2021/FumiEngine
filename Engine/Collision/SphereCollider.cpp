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

	// 押し出しベクトルをリセット
	reject_ = { 0.0f, 0.0f, 0.0f };

	// オブジェクト3Dが紐づけられていたら
	if (object_) {
		Sphere::center = object_->GetPosition() + offset_;
	}

	else {
		Sphere::center = offset_;
	}

	Sphere::radius = radius_;
}