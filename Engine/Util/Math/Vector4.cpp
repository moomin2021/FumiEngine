#include "Vector4.h"
#include "Vector3.h"

// 引数なしのコンストラクタ
Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

// 引数ありのコンストラクタ
Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vector4::Vector4(const Vector3& vec, float a) : x(vec.x), y(vec.y), z(vec.z), w(a) {}

Vector4 Vector4::operator+() const
{
	return *this;
}

Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

Vector4& Vector4::operator+=(const Vector4& f)
{
	x += f.x;
	y += f.y;
	z += f.z;
	w += f.w;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4& f)
{
	x -= f.x;
	y -= f.y;
	z -= f.z;
	w -= f.w;
	return *this;
}

Vector4& Vector4::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

Vector4& Vector4::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

const Vector4 operator+(const Vector4& f1, const Vector4& f2)
{
	Vector4 tmp(f1);
	return tmp += f2;
}

const Vector4 operator-(const Vector4& f1, const Vector4& f2)
{
	Vector4 tmp(f1);
	return tmp += f2;
}

const Vector4 operator*(const Vector4& f, float s)
{
	Vector4 tmp(f);
	return tmp *= s;
}

const Vector4 operator*(float s, const Vector4& f)
{
	return f * s;
}

const Vector4 operator/(const Vector4& f, float s)
{
	Vector4 tmp(f);
	return tmp /= s;
}
