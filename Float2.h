#pragma once
struct Float2
{
	float x;// -> x����
	float y;// -> y����

	// �R���X�g���N�^
	Float2();// -----------------> �����Ȃ�
	Float2(float x, float y);// -> ��������

	// �P�����Z�q�I�[�o�[���[�h
	Float2 operator+() const;
	Float2 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	Float2& operator+=(const Float2& f);
	Float2& operator-=(const Float2& f);
	Float2& operator*=(float s);
	Float2& operator/=(float s);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ������Ă���
const Float2 operator+(const Float2& f1, const Float2& f2);
const Float2 operator-(const Float2& f1, const Float2& f2);
const Float2 operator*(const Float2& f, float s);
const Float2 operator*(float s, const Float2& f);
const Float2 operator/(const Float2& f, float s);