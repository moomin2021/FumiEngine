#include "Camera.h"

Camera::Camera() :
#pragma region 初期化リスト
	eye_{0.0f, 0.0f, 0.0f},// ----> カメラの座標
	target_{0.0f, 0.0f, 0.0f},// -> カメラの注視点
	up_{0.0f, 1.0f, 0.0f},// -----> カメラの上方向ベクトル

	matView_{}// -> ビュー変換行列
#pragma endregion
{}

void Camera::Update() {
	// --ビュー変換行列-- //
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}