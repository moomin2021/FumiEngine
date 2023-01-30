#pragma once
// --数学関数-- //
#include <DirectXMath.h>
using namespace DirectX;

class Camera {
public:
	XMFLOAT3 eye_;// ----> カメラの位置
	XMFLOAT3 target_;// -> カメラの注視点
	XMFLOAT3 up_;// -----> カメラの上方向ベクトル

private:
	XMMATRIX matView_;// -> ビュー変換行列
	XMMATRIX matProjection_;// -> 透視投影行列

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
	inline const XMMATRIX& GetMatView() { return matView_; }
	inline const XMMATRIX& GetMatProjection() { return matProjection_; }
#pragma endregion
};