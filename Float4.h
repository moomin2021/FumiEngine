#pragma once
struct Float4
{
	float x;// -> x����
	float y;// -> y����
	float z;// -> z����
	float w;// -> w����

	// �R���X�g���N�^
	Float4();// -----------------------------------> �����Ȃ�
	Float4(float x, float y, float z, float w);// -> ��������

	// �P�����Z�q�I�[�o�[���[�h
	Float4 operator+() const;
	Float4 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	Float4& operator+=(const Float4& f);
	Float4& operator-=(const Float4& f);
	Float4& operator*=(float s);
	Float4& operator/=(float s);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const Float4 operator+(const Float4& f1, const Float4& f2);
const Float4 operator-(const Float4& f1, const Float4& f2);
const Float4 operator*(const Float4& f, float s);
const Float4 operator*(float s, const Float4& f);
const Float4 operator/(const Float4& f, float s);