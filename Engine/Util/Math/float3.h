#pragma once

class Vector3;

struct float3
{
	// x�����Ay����, z����
	float x, y, z;

	// �R���X�g���N�^
	float3();// --------------------------> �����Ȃ�
	float3(float x, float y, float z);// -> ��������
	float3(Vector3 pos);

	// �P�����Z�q�I�[�o�[���[�h
	float3 operator+() const;
	float3 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	float3& operator+=(const float3& f);
	float3& operator-=(const float3& f);
	float3& operator*=(float s);
	float3& operator/=(float s);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const float3 operator+(const float3& f1, const float3& f2);
const float3 operator-(const float3& f1, const float3& f2);
const float3 operator*(const float3& f, float s);
const float3 operator*(float s, const float3& f);
const float3 operator/(const float3& f, float s);