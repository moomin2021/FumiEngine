#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class CubeCollider : public BaseCollider, public Cube {
#pragma region �����o�ϐ�
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	float3 offset_ = { 0.0f, 0.0f, 0.0f };

	// �e�����̔��a
	float3 radius_ = { 0.0f, 0.0f, 0.0f };
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="offset"> �I�t�Z�b�g </param>
	/// <param name="radius"> ���a </param>
	CubeCollider(const float3& offset = { 0.0f, 0.0f, 0.0f }, const float3& radius = { 1.0f, 1.0f, 1.0f });

	// �X�V����
	void Update() override;
#pragma endregion
};