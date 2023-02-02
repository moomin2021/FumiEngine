#pragma once
#include "Vector3.h"
#include "Matrix4.h"

struct Quaternion {
	float x;
	float y;
	float z;
	float w;

	Quaternion(float x, float y, float z, float w);

	// �P�����Z�q�I�[�o�[���[�h
	Quaternion operator+() const;
	Quaternion operator-() const;

	// ������Z�q�I�[�o�[���[�h
	Quaternion& operator+=(const Quaternion& q);
	Quaternion& operator-=(const Quaternion& q);
	Quaternion& operator*=(float s);
	Quaternion& operator/=(float s);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator*(const Quaternion& q, float s);
const Quaternion operator*(float s, const Quaternion& q);
const Quaternion operator/(const Quaternion& q, float s);

// Quaternion�̐�
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

// �P��Quaternion��Ԃ�
Quaternion IdentityQuaternion();

// ����Quaternion��Ԃ�
Quaternion Conjugate(const Quaternion& q);

// Quaternion��norm��Ԃ�
float Norm(const Quaternion& q);

// ���K������Quaternion��Ԃ�
Quaternion Normalise(const Quaternion& q);

// �tQuaternion��Ԃ�
Quaternion Inverse(const Quaternion& q);

// �C�ӎ���]��\��Quaternion�̐���
Quaternion MakeAxisAngle(const Vector3& axis, float angle);

// �x�N�g����Quaternion�ŉ�]���������ʂ̃x�N�g�������߂�
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

// Quaternion�����]�s������߂�
Matrix4 MakeRotateMatrix(const Quaternion& q);

// ���ʐ��`���
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

// u����v�ւ̉�]�̐���
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);