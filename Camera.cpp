#include "Camera.h"
#include "Util.h"
#include "WinAPI.h"

Camera::Camera() :
#pragma region 初期化リスト
	eye_{0.0f, 0.0f, 0.0f},// ----> カメラの座標
	target_{0.0f, 0.0f, 0.0f},// -> カメラの注視点
	up_{0.0f, 1.0f, 0.0f},// -----> カメラの上方向ベクトル

	matView_{},// -> ビュー変換行列
	matProjection_{}// -> 透視投影行列
#pragma endregion
{
	// --透視投影行列の計算-- //
	matProjection_ = XMMatrixPerspectiveFovLH(
		Util::Degree2Radian(45.0f),// -----------> 上下画角45度
		(float)WinAPI::GetWidth() / WinAPI::GetHeight(),// -> アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f// ------------------------> 前端、奥端
	);
}

void Camera::Update() {
	// --ビュー変換行列-- //
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}