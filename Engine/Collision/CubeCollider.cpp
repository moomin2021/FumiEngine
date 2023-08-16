#include "CubeCollider.h"

CubeCollider::CubeCollider(const float3& offset, const float3& radius) {
	offset_ = offset;
	radius_ = radius;

	// �`��^�C�v�𗧕��̂ɐݒ�
	shapeType_ = SHAPE_CUBE;
}

void CubeCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;
	reject_ = { 0.0f, 0.0f, 0.0f };

	// �I�u�W�F�N�g3D���R�Â����Ă�����
	if (object_) {
		// ���[���h�s�񂩂烏�[���h���W�𒊏o
		const Matrix4& matWorld = object_->GetMatWorld();
		Cube::center = Vector3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]) + Vector3(offset_);
	}

	else {
		Cube::center = offset_;
	}

	Cube::radius = radius_;
}