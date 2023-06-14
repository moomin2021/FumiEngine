#pragma once
#include "fMath.h"

class Camera {
public:
	float3 eye_;// ----> �J�����̈ʒu
	float3 target_;// -> �J�����̒����_
	float3 up_;// -----> �J�����̏�����x�N�g��

private:
	Matrix4 matView_;// -> �r���[�ϊ��s��
	Matrix4 matProjection_;// -> �������e�s��

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Camera();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

#pragma region �Q�b�^�[
	// �r���[�s��̎擾
	inline const Matrix4& GetMatView() { return matView_; }
	inline const Matrix4& GetMatProjection() { return matProjection_; }
#pragma endregion
};