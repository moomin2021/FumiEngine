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

	// �Փ˔��葮��
	uint16_t attribute_ = 0;

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
#pragma endregion

#pragma region �Z�b�^�[�֐�
	// �`��^�C�v��ݒ�
	inline void SetShapeType(CollisionShapeType shapeType) { shapeType_ = shapeType; }

	// �Փ˔��葮����ݒ�
	inline void SetAttribute(uint16_t attribute) { attribute_ = attribute; }

	// �I�u�W�F�N�g��ݒ�
	inline void SetObject3D(Object3D* object) { object_ = object; }

	// �Փ˃t���O��ݒ�
	inline void SetIsHit(bool flag) { isHit_ = flag; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// �`��^�C�v���擾
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	// �Փ˔��葮����ݒ�
	inline uint16_t GetAttribute() { return attribute_; }

	// �Փ˃t���O��ݒ�
	inline bool GetIsHit() { return isHit_; }
#pragma endregion
};