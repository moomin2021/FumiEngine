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

	// �����o���x�N�g�������Z�b�g
	reject_ = { 0.0f, 0.0f, 0.0f };

	// �I�u�W�F�N�g3D���R�Â����Ă�����
	if (object_) {
		Sphere::center = object_->GetPosition() + offset_;
	}

	else {
		Sphere::center = offset_;
	}

	Sphere::radius = radius_;
}