#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "float3.h"

class SphereCollider : public BaseCollider, public Sphere
{
#pragma region �����o�ϐ�
private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	float3 offset_;

	// ���a
	float radius_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="offset"> �I�t�Z�b�g </param>
	/// <param name="radius"> ���a </param>
	SphereCollider(float3 offset = { 0.0f, 0.0f, 0.0f }, float radius = 1.0f);

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
	/// ���a��ݒ�
	/// </summary>
	/// <param name="radius"> ���a </param>
	void SetRadius(float radius) { radius_ = radius; }
#pragma endregion
};