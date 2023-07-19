#pragma once
#include "Object3D.h"
#include "SphereCollider.h"

#include <memory>

class BaseEnemy
{
#pragma region �����o�ϐ�
protected:
	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> object_;

	// �R���C�_�[
	std::unique_ptr<SphereCollider> collider_;

	// �����t���O
	bool isAlive_ = true;
#pragma endregion

#pragma region �����o�֐�
public:
	// �X�V����
	virtual void Update() = 0;

	// �`�揈��
	virtual void Draw() = 0;
#pragma endregion

#pragma region �Q�b�^�[�֐�
	/// <summary>
	/// �����t���O���擾
	/// </summary>
	/// <returns> �t���O </returns>
	inline bool GetIsAlive() { return isAlive_; }
#pragma endregion
};