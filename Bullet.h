#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Collision.h"

#include <memory>

class Bullet {
#pragma region �����o�ϐ�
private:
	// �e�̃��f��
	static Model* mBullet_;

	// �e�̃I�u�W�F�N�g
	std::unique_ptr<Object3D> oBullet_;

	// �Փ˔���p
	SphereCol col_;

	// �e�̈ړ�����
	Vector3 moveVec_;

	// �e�̑��x
	float bulletSpd_;

	// �����t���O
	bool isAlive_;

	// ��������
	uint16_t aliveCounter_;	// ��������̃J�E���g
	uint16_t maxCount_;		// �ő�J�E���g
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="moveVec"> �e�̈ړ����� </param>
	/// <param name="iniPos"> �������W </param>
	Bullet(const float3& iniPos, const Vector3& moveVec);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();
private:
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	/// <summary>
	/// ���f����ݒ�
	/// </summary>
	/// <param name="model"> ���f�� </param>
	static void SetModel(Model* model) { mBullet_ = model; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	/// <summary>
	/// �����t���O���擾
	/// </summary>
	/// <returns> �����t���O </returns>
	bool GetIsAlive() { return isAlive_; }

	inline const SphereCol& GetSphereCol() { return col_; }
#pragma endregion
};