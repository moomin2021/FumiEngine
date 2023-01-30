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
	XMMATRIX matProjection_;// -> �������e�s��

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
	inline const XMMATRIX& GetMatView() { return matView_; }
	inline const XMMATRIX& GetMatProjection() { return matProjection_; }
#pragma endregion
};