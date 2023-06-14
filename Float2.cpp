#include "float2.h"

// 引数無しのコンストラクタ
float2::float2() : x(0.0f), y(0.0f) {}

// 引数ありのコンストラクタ
float2::float2(float x, float y) : x(x), y(y) {}

// 単項演算子+を使えるようにする
float2 float2::operator+() const
{
    return *this;
}

// 単項演算子-を使えるようにする
float2 float2::operator-() const
{
    return float2(-x, -y);
}

// +=を使用できるようにする
float2& float2::operator+=(const float2& f)
{
    x += f.x;
    y += f.y;
    return *this;
}

// -=を使用できるようにする
float2& float2::operator-=(const float2& f)
{
    x -= f.x;
    y -= f.y;
    return *this;
}

// ベクトルをs倍にする
float2& float2::operator*=(float s)
{
    x *= s;
    y *= s;
    return *this;
}

// ベクトルを1/s倍にする
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
