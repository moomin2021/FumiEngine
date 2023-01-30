#pragma once
// --���w�֐�-- //
#include <DirectXMath.h>
using namespace DirectX;
#include "fMath.h"

class Camera {
public:
	Float3 eye_;// ----> �J�����̈ʒu
	Float3 target_;// -> �J�����̒����_
	Float3 up_;// -----> �J�����̏�����x�N�g��

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