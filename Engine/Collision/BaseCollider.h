#pragma once
#include "Object3D.h"

enum CollisionShapeType {
	SHAPE_UNKNOWN = -1,	// ���ݒ�
	SHAPE_SPHERE,		// ��
	SHAPE_MESH,			// ���b�V��
};

class BaseCollider {
#pragma region �����o�ϐ�
protected:
	// �`��^�C�v
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	// �C�ӂŕR�t����I�u�W�F�N�g
	Object3D* object_ = nullptr;

	// �Փ˃t���O
	bool isHit_ = false;
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
#pragma endregion

#pragma region �Z�b�^�[�֐�
	/// <summary>
	/// �Փ˃t���O��ݒ�
	/// </summary>
	/// <param name="frag"></param>
	void SetIsHit(bool frag) { isHit_ = frag; }
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
#pragma endregion
};