#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "float3.h"

class RayCollider : public BaseCollider, public Ray
{
#pragma region �����o�ϐ�
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	float3 offset_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="offset"> �I�t�Z�b�g </param>
	/// <param name="radius"> ���a </param>
	RayCollider(float3 offset = { 0.0f, 0.0f, 0.0f }, Vector3 dir = { 0.0f, 0.0f, 1.0f });

	// �X�V����
	void Update() override;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	/// <summary>
	/// �I�t�Z�b�g��ݒ�
	/// </summary>
	/// <param name="offset"> �I�t�Z�b�g </param>
	void SetOffset(const float3& offset) { offset_ = offset; }

	/// <summary>
	/// ������ݒ�
	/// </summary>
	/// <param name="dir"> ���� </param>
	void SetDir(const Vector3& dir) { Ray::dir = dir; }
#pragma endregion
};