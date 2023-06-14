#include "float2.h"

// ���������̃R���X�g���N�^
float2::float2() : x(0.0f), y(0.0f) {}

// ��������̃R���X�g���N�^
float2::float2(float x, float y) : x(x), y(y) {}

// �P�����Z�q+���g����悤�ɂ���
float2 float2::operator+() const
{
    return *this;
}

// �P�����Z�q-���g����悤�ɂ���
float2 float2::operator-() const
{
    return float2(-x, -y);
}

// +=���g�p�ł���悤�ɂ���
float2& float2::operator+=(const float2& f)
{
    x += f.x;
    y += f.y;
    return *this;
}

// -=���g�p�ł���悤�ɂ���
float2& float2::operator-=(const float2& f)
{
    x -= f.x;
    y -= f.y;
    return *this;
}

// �x�N�g����s�{�ɂ���
float2& float2::operator*=(float s)
{
    x *= s;
    y *= s;
    return *this;
}

// �x�N�g����1/s�{�ɂ���
float2& float2::operator/=(float s)
{
    x /= s;
    y /= s;
    return *this;
}

const float2 operator+(const float2& f1, const float2& f2)
{
    float2 tmp(f1);
    return tmp += f2;
}

const float2 operator-(const float2& f1, const float2& f2)
{
    float2 tmp(f1);
    return tmp -= f2;
}

const float2 operator*(const float2& f, float s)
{
    float2 tmp(f);
    return tmp *= s;
}

const float2 operator*(float s, const float2& f)
{
    return f * s;
}

const float2 operator/(const float2& f, float s)
{
    float2 tmp(f);
    return tmp /= s;
}
