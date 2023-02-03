#include "Float2.h"

// ���������̃R���X�g���N�^
Float2::Float2() : x(0.0f), y(0.0f) {}

// ��������̃R���X�g���N�^
Float2::Float2(float x, float y) : x(x), y(y) {}

// �P�����Z�q+���g����悤�ɂ���
Float2 Float2::operator+() const
{
    return *this;
}

// �P�����Z�q-���g����悤�ɂ���
Float2 Float2::operator-() const
{
    return Float2(-x, -y);
}

// +=���g�p�ł���悤�ɂ���
Float2& Float2::operator+=(const Float2& f)
{
    x += f.x;
    y += f.y;
    return *this;
}

// -=���g�p�ł���悤�ɂ���
Float2& Float2::operator-=(const Float2& f)
{
    x -= f.x;
    y -= f.y;
    return *this;
}

// �x�N�g����s�{�ɂ���
Float2& Float2::operator*=(float s)
{
    x *= s;
    y *= s;
    return *this;
}

// �x�N�g����1/s�{�ɂ���
Float2& Float2::operator/=(float s)
{
    x /= s;
    y /= s;
    return *this;
}

const Float2 operator+(const Float2& f1, const Float2& f2)
{
    Float2 tmp(f1);
    return tmp += f2;
}

const Float2 operator-(const Float2& f1, const Float2& f2)
{
    Float2 tmp(f1);
    return tmp -= f2;
}

const Float2 operator*(const Float2& f, float s)
{
    Float2 tmp(f);
    return tmp *= s;
}

const Float2 operator*(float s, const Float2& f)
{
    return f * s;
}

const Float2 operator/(const Float2& f, float s)
{
    Float2 tmp(f);
    return tmp /= s;
}
