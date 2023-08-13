#include "float3.h"
#include "Vector3.h"

// 引数なしのコンストラクタ
float3::float3() : x(0.0f), y(0.0f), z(0.0f) {}

// 引数ありのコンストラクタ
float3::float3(float x, float y, float z) : x(x), y(y), z(z) {}

float3::float3(Vector3 pos)
{
	x = pos.x;
	y = pos.y;
	z = pos.z;
}

float3 float3::operator+() const
{
	return *this;
}

float3 float3::operator-() const
{
	return float3(-x, -y, -z);
}

float3& float3::operator+=(const float3& f)
{
	x += f.x;
	y += f.y;
	z += f.z;
	return *this;
}

float3& float3::operator-=(const float3& f)
{
	x -= f.x;
	y -= f.y;
	z -= f.z;
	return *this;
}

float3& float3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

float3& float3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const float3 operator+(const float3& f1, const float3& f2)
{
	float3 tmp(f1);
	return tmp += f2;
}

const float3 operator+(const Vector3& v1, const float3& f1)
{
	return float3(v1.x + f1.x, v1.y + f1.y, v1.z + f1.z);
}

const float3 operator+(const float3& f1, const Vector3& v1)
{
	return float3(v1.x + f1.x, v1.y + f1.y, v1.z + f1.z);
}

const float3 operator-(const float3& f1, const float3& f2)
{
	float3 tmp(f1);
	return tmp -= f2;
}

const float3 operator-(const float3& f, const Vector3& v)
{
	float3 tmp(f);
	return tmp -= v;
}

const float3 operator*(const float3& f, float s)
{
	float3 tmp(f);
	return tmp *= s;
}

const float3 operator*(float s, const float3& f)
{
	return f * s;
}

const float3 operator/(const float3& f, float s)
{
	float3 tmp(f);
	return tmp /= s;
}
