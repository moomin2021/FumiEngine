#pragma once
#include "fMath.h"

class Camera {
public:
	float3 eye_;// ----> カメラの位置
	float3 target_;// -> カメラの注視点
	float3 up_;// -----> カメラの上方向ベクトル

private:
	Matrix4 matView_;// -> ビュー変換行列
	Matrix4 matProjection_;// -> 透視投影行列

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

#pragma region ゲッター
	// ビュー行列の取得
	inline const Matrix4& GetMatView() { return matView_; }
	inline const Matrix4& GetMatProjection() { return matProjection_; }
#pragma endregion
};