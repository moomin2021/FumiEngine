#pragma once
#include "Object3D.h"
#include "Collision.h"

#include <memory>

class Enemy {
#pragma region �����o�ϐ�
private:
	// ���f��
	static Model* model_;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> obj_;

	// �Փ˔���p
	//SphereCol col_;

	// HP
	uint16_t hp_;

	// �����t���O
	bool isAlive_;

	uint16_t damageCounter_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Enemy();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// HP�����炷
	/// </summary>
	/// <param name="reduceValue"> ���炷�l </param>
	void ReduceHP(uint16_t reduceValue);
private:

#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	/// <summary>
	/// ���f����ݒ�
	/// </summary>
	/// <param name="model"> ���f�� </param>
	static inline void SetModel(Model* model) { model_ = model; }

	/// <summary>
	/// �I�u�W�F�N�g�̍��W(XYZ)��ݒ�
	/// </summary>
	/// <param name="pos"> ���W(XYZ) </param>
	inline void SetPosition(const float3& pos) { obj_->SetPosition(pos); }

	/// <summary>
	/// �I�u�W�F�N�g�̊g�k(XYZ)��ݒ�
	/// </summary>
	/// <param name="scale"> �g�k(XYZ) </param>
	inline void SetScale(const float3& scale) { obj_->SetScale(scale); }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	//inline const SphereCol& GetSphereCol() { return col_; }

	inline bool GetIsAlive() { return isAlive_; }
#pragma endregion
};