#pragma once
#include "Matrix4.h"
#include "float3.h"

class Camera {
#pragma region �����o�ϐ�
private:
	// �J�����f�[�^
	float3 eye_;	// �J�����̈ʒu
	float3 target_;	// �J�����̒����_
	float3 up_;		// �J�����̏�����x�N�g��

	// �s��
	Matrix4 matView_;		// �r���[�ϊ��s��
	Matrix4 matProjection_;	// �������e�s��
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Camera();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

#pragma region �Z�b�^�[�֐�
	/// <summary>
	/// �J�����̈ʒu��ݒ�
	/// </summary>
	/// <param name="eye"> �J�����̈ʒu </param>
	inline void SetEye(const float3& eye) { eye_ = eye; }

	/// <summary>
	/// �J�����̒����_��ݒ�
	/// </summary>
	/// <param name="target"> �J�����̒����_ </param>
	inline void SetTarget(const float3& target) { target_ = target; }

	/// <summary>
	/// �J�����̏�����x�N�g��
	/// </summary>
	/// <param name="up"> �J�����̏�����x�N�g�� </param>
	inline void SetUp(const float3& up) { up_ = up; }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="fovAngleY"></param>
	void SetFovAngleY(float fovAngleY);
#pragma endregion

#pragma region �Q�b�^�[�֐�
	/// <summary>
	/// �J�����̈ʒu���擾
	/// </summary>
	/// <returns> �J�����̈ʒu </returns>
	inline const float3& GetEye() { return eye_; }

	/// <summary>
	/// �J�����̒����_���擾
	/// </summary>
	/// <returns> �J�����̒����_ </returns>
	inline const float3& GetTarget() { return target_; }

	/// <summary>
	/// �J�����̏�������擾
	/// </summary>
	/// <returns> �J�����̏���� </returns>
	inline const float3& GetUp() { return up_; }

	/// <summary>
	/// �r���[�ϊ��s����擾
	/// </summary>
	/// <returns> �r���[�ϊ��s�� </returns>
	inline const Matrix4& GetMatView() { return matView_; }

	/// <summary>
	/// �������e�s����擾
	/// </summary>
	/// <returns> �������e�s�� </returns>
	inline const Matrix4& GetMatProjection() { return matProjection_; }
#pragma endregion

#pragma endregion
};