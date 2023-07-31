#include "SphereCollider.h"
#include "Matrix4.h"

SphereCollider::SphereCollider(float3 offset, float radius) :
	offset_(offset), radius_(radius)
{
	// �`��^�C�v�����ɐݒ�
	shapeType_ = SHAPE_SPHERE;
}

void SphereCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;
	reject_ = { 0.0f, 0.0f, 0.0f };

	// �I�u�W�F�N�g3D���R�Â����Ă�����
	if (object_) {
		// ���[���h�s�񂩂烏�[���h���W�𒊏o
		const Matrix4& matWorld = object_->GetMatWorld();
		Sphere::center = Vector3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]) + Vector3(offset_);
	}

	else {
		Sphere::center = offset_;
	}

	Sphere::radius = radius_;
}