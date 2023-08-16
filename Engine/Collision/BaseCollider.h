#pragma once
#include "Object3D.h"

enum CollisionShapeType {
	SHAPE_UNKNOWN = -1,	// ���ݒ�
	SHAPE_SPHERE,		// ��
	SHAPE_MESH,			// ���b�V��
	SHAPE_RAY,			// ���C
};

class BaseCollider {
public:
	friend class CollisionManager;

#pragma region �����o�ϐ�
protected:
	// �`��^�C�v
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	// �C�ӂŕR�t����I�u�W�F�N�g
	Object3D* object_ = nullptr;

	// �Փ˃t���O
	bool isHit_ = false;

	// �����蔻�葮��
	uint16_t attribute_ = 0;

	// �Փˑ���Ƃ̌�_
	float3 inter_ = { 0.0f, 0.0f, 0.0f };

	// �����o���x�N�g��
	Vector3 reject_ = { 0.0f, 0.0f, 0.0f };
#pragma endregion

#pragma region �����o�֐�
public:
	// �f�X�g���N�^
	virtual ~BaseCollider() = default;

	// �X�V����
	virtual void Update() = 0;

	/// <summary>
	/// �I�u�W�F�N�g3D�ƕR�t����
	/// </summary>
	/// <param name="object"> �I�u�W�F�N�g3D </param>
	inline void LinkObject3D(Object3D* object) { object_ = object; }

	/// <summary>
	/// �I�u�W�F�N�g3D�Ƃ̕R�Â�������
	/// </summary>
	inline void CutLinkObject3D() { object_ = nullptr; }

	/// <summary>
	/// �����蔻�葮����ǉ�
	/// </summary>
	/// <param name="attribute"> �����蔻�葮�� </param>
	inline void AddAttribute(uint16_t attribute) { attribute_ |= attribute; }

	/// <summary>
	/// �����蔻�葮�����폜
	/// </summary>
	/// <param name="attribute"> �����蔻�葮�� </param>
	inline void RemoveAttribute(uint16_t attribute) { attribute_ &= !attribute; }
#pragma endregion

#pragma region �Z�b�^�[�֐�
	/// <summary>
	/// �Փ˃t���O��ݒ�
	/// </summary>
	/// <param name="frag"></param>
	void SetIsHit(bool frag) { isHit_ = frag; }

	/// <summary>
	/// �����蔻�葮����ݒ�
	/// </summary>
	/// <param name="attribute"> �����蔻�葮�� </param>
	inline void SetAttribute(uint16_t attribute) { attribute_ = attribute; }

	/// <summary>
	/// �Փˑ���̌�_��ݒ�
	/// </summary>
	/// <param name="inter"> ��_ </param>
	inline void SetInter(const float3& inter) { inter_ = inter; }

	/// <summary>
	/// �����o���x�N�g����ݒ�
	/// </summary>
	/// <param name="reject"> �����o���x�N�g�� </param>
	inline void SetReject(const Vector3& reject) { reject_ = reject; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	/// <summary>
	/// �`��^�C�v���擾
	/// </summary>
	/// <returns></returns>
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	/// <summary>
	/// �Փ˃t���O���擾
	/// </summary>
	/// <returns> �Փ˃t���O </returns>
	inline bool GetIsHit() { return isHit_; }

	/// <summary>
	/// �Փˑ���̌�_���擾
	/// </summary>
	/// <returns> ��_ </returns>
	inline const float3& GetInter() { return inter_; }

	/// <summary>
	/// �����o���x�N�g�����擾
	/// </summary>
	/// <returns> �����o���x�N�g�� </returns>
	inline const Vector3& GetReject() { return reject_; }
#pragma endregion
};