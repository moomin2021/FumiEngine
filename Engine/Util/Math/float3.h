#pragma once

class Vector3;

struct float3
{
	// x成分、y成分, z成分
	float x, y, z;

	// コンストラクタ
	float3();// --------------------------> 引数なし
	float3(float x, float y, float z);// -> 引数あり
	float3(Vector3 pos);

	// 単項演算子オーバーロード
	float3 operator+() const;
	float3 operator-() const;

	// 代入演算子オーバーロード
	float3& operator+=(const float3& f);
	float3& operator-=(const float3& f);
	float3& operator*=(float s);
	float3& operator/=(float s);
};

// 2項演算子オーバーロード
// ※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備している
const float3 operator+(const float3& f1, const float3& f2);
const float3 operator-(const float3& f1, const float3& f2);
const float3 operator*(const float3& f, float s);
const float3 operator*(float s, const float3& f);
const float3 operator/(const float3& f, float s);