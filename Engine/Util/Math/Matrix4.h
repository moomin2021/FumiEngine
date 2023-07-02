#pragma once
#include "float3.h"

// --Matrix4構造体-- //
struct Matrix4 {
	float m[4][4];
};

// --単位行列を求める-- //
Matrix4 Matrix4Identity();

// --拡大縮小行列の設定-- //
Matrix4 Matrix4Scale(const float3& s);

// --回転行列の設定-- //
Matrix4 Matrix4RotateX(float angle);
Matrix4 Matrix4RotateY(float angle);
Matrix4 Matrix4RotateZ(float angle);

// --平行移動行列の作成-- //
Matrix4 Matrix4Translate(const float3& t);

// --座標変換（ベクトルと行列の掛け算をする）-- //
float3 Matrix4Transform(const float3& v, const Matrix4& m);

// --代入演算子オーバーロード-- //
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

// --2項演算子オーバーロード-- //
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const float3 operator*(const float3& v, const Matrix4& m);