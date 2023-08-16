#include "CubeCollider.h"

CubeCollider::CubeCollider(const float3& offset, const float3& radius) {
	offset_ = offset;
	radius_ = radius;

	// 形状タイプを立方体に設定
	shapeType_ = SHAPE_CUBE;
}

void CubeCollider::Update()
{
	// 衝突フラグを初期化
	isHit_ = false;
	reject_ = { 0.0f, 0.0f, 0.0f };

	// オブジェクト3Dが紐づけられていたら
	if (object_) {
		// ワールド行列からワールド座標を抽出
		const Matrix4& matWorld = object_->GetMatWorld();
		Cube::center = Vector3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]) + Vector3(offset_);
	}

	else {
		Cube::center = offset_;
	}

	Cube::radius = radius_;
}