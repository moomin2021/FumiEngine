#pragma once
struct float2
{
	// x�����Ay����
	float x, y;

	// �R���X�g���N�^
	float2();// -----------------> �����Ȃ�
	float2(float x, float y);// -> ��������

	// �P�����Z�q�I�[�o�[���[�h
	float2 operator+() const;
	float2 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	float2& operator+=(const float2& f);
	float2& operator-=(const float2& f);
	float2& operator*=(float s);
	float2& operator/=(float s);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ������Ă���
const float2 operator+(const float2& f1, const float2& f2);
const float2 operator-(const float2& f1, const float2& f2);
const float2 operator*(const float2& f, float s);
const float2 operator*(float s, const float2& f);
const float2 operator/(const float2& f, float s);