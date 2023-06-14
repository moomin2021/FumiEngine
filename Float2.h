#pragma once
struct float2
{
	// x成分、y成分
	float x, y;

	// コンストラクタ
	float2();// -----------------> 引数なし
	float2(float x, float y);// -> 引数あり

	// 単項演算子オーバーロード
	float2 operator+() const;
	float2 operator-() const;

	// 代入演算子オーバーロード
	float2& operator+=(const float2& f);
	float2& operator-=(const float2& f);
	float2& operator*=(float s);
	float2& operator/=(float s);
};

// 2項演算子オーバーロード
// ※いろんな引数のパターンに対応するため、以下のように準備している
const float2 operator+(const float2& f1, const float2& f2);
const float2 operator-(const float2& f1, const float2& f2);
const float2 operator*(const float2& f, float s);
const float2 operator*(float s, const float2& f);
const float2 operator/(const float2& f, float s);