#include "RayCollider.h"

RayCollider::RayCollider(float3 offset, Vector3 dir) :
	offset_(offset)
{
	shapeType_ = SHAPE_RAY;

	Ray::dir = dir;
}

void RayCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;

	// �I�u�W�F�N�g3D���R�Â����Ă�����
	if (object_) {
		// ���[���h�s�񂩂烏�[���h���W�𒊏o
		const Matrix4& matWorld = object_->GetMatWorld();
		Ray::start = Vector3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]) + Vector3(offset_);
	}

	else {
		Ray::start = offset_;
	}
}