#pragma once
struct Float3
{
	float x;// -> x����
	float y;// -> y����
	float z;// -> z����

	// �R���X�g���N�^
	Float3();// --------------------------> �����Ȃ�
	Float3(float x, float y, float z);// -> ��������

	// �P�����Z�q�I�[�o�[���[�h
	Float3 operator+() const;
	Float3 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	Float3& operator+=(const Float3& f);
	Float3& operator-=(const Float3& f);
	Float3& operator*=(float s);
	Float3& operator/=(float s);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const Float3 operator+(const Float3& f1, const Float3& f2);
const Float3 operator-(const Float3& f1, const Float3& f2);
const Float3 operator*(const Float3& f, float s);
const Float3 operator*(float s, const Float3& f);
const Float3 operator/(const Float3& f, float s);