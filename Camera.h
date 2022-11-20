#pragma once
// --���w�֐�-- //
#include <DirectXMath.h>
using namespace DirectX;

class Camera {
public:
	XMFLOAT3 eye_;// ----> �J�����̈ʒu
	XMFLOAT3 target_;// -> �J�����̒����_
	XMFLOAT3 up_;// -----> �J�����̏�����x�N�g��

private:
	XMMATRIX matView_;// -> �r���[�ϊ��s��

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
	const XMMATRIX& GetMatView() { return matView_; }
#pragma endregion
};