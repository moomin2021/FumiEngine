#pragma once
#include "Float3.h"

// --Matrix4�\����-- //
struct Matrix4 {
	float m[4][4];
};

// --�P�ʍs������߂�-- //
Matrix4 Matrix4Identity();

// --�g��k���s��̐ݒ�-- //
Matrix4 Matrix4Scale(const Float3& s);

// --��]�s��̐ݒ�-- //
Matrix4 Matrix4RotateX(float angle);
Matrix4 Matrix4RotateY(float angle);
Matrix4 Matrix4RotateZ(float angle);

// --���s�ړ��s��̍쐬-- //
Matrix4 Matrix4Translate(const Float3& t);

// --���W�ϊ��i�x�N�g���ƍs��̊|���Z������j-- //
Float3 Matrix4Transform(const Float3& v, const Matrix4& m);

// --������Z�q�I�[�o�[���[�h-- //
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

// --2�����Z�q�I�[�o�[���[�h-- //
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Float3 operator*(const Float3& v, const Matrix4& m);