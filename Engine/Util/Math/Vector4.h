#pragma once
class Vector3;

struct Vector4
{
	// x成分、y成分、z成分、w成分
	float x, y, z, w;

	// コンストラクタ
	Vector4();// -----------------------------------> 引数なし
	Vector4(float x, float y, float z, float w);// -> 引数あり
	Vector4(const Vector3& vec, float a);

	// 単項演算子オーバーロード
	Vector4 operator+() const;
	Vector4 operator-() const;

	// 代入演算子オーバーロード
	Vector4& operator+=(const Vector4& f);
	Vector4& operator-=(const Vector4& f);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);
};

// 2項演算子オーバーロード
// ※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備している
const Vector4 operator+(const Vector4& f1, const Vector4& f2);
const Vector4 operator-(const Vector4& f1, const Vector4& f2);
const Vector4 operator*(const Vector4& f, float s);
const Vector4 operator*(float s, const Vector4& f);
const Vector4 operator/(const Vector4& f, float s);
