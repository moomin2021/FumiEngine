#include "Camera.h"

Camera::Camera() :
#pragma region ���������X�g
	eye_{0.0f, 0.0f, 0.0f},// ----> �J�����̍��W
	target_{0.0f, 0.0f, 0.0f},// -> �J�����̒����_
	up_{0.0f, 1.0f, 0.0f},// -----> �J�����̏�����x�N�g��

	matView_{}// -> �r���[�ϊ��s��
#pragma endregion
{}

void Camera::Update() {
	// --�r���[�ϊ��s��-- //
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}