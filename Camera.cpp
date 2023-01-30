#include "Camera.h"
#include "Util.h"
#include "WinAPI.h"

Camera::Camera() :
#pragma region ���������X�g
	eye_{0.0f, 0.0f, 0.0f},// ----> �J�����̍��W
	target_{0.0f, 0.0f, 0.0f},// -> �J�����̒����_
	up_{0.0f, 1.0f, 0.0f},// -----> �J�����̏�����x�N�g��

	matView_{},// -> �r���[�ϊ��s��
	matProjection_{}// -> �������e�s��
#pragma endregion
{
	// --�������e�s��̌v�Z-- //
	matProjection_ = XMMatrixPerspectiveFovLH(
		Util::Degree2Radian(45.0f),// -----------> �㉺��p45�x
		(float)WinAPI::GetWidth() / WinAPI::GetHeight(),// -> �A�X�y�N�g��i��ʉ���/��ʏc���j
		0.1f, 1000.0f// ------------------------> �O�[�A���[
	);
}

void Camera::Update() {
	// --�r���[�ϊ��s��-- //
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}