#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Collision.h"
#include "SphereCollider.h"

#include <memory>

enum BulletType {
	PLAYER,
	ENEMY0,
};

class Bullet {
#pragma region �\����
private:
	struct Data {
		std::unique_ptr<Object3D> object = nullptr;
		std::unique_ptr<SphereCollider> col = nullptr;
		Vector3 moveVec = {};
		float bulletSpd = 0.0f;
		uint16_t aliveTime = 0;
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// �f�[�^
	Data data_ = {};

	// �����t���O
	bool isAlive_ = true;

	// �������ꂽ����
	uint64_t generatedTime_ = 0;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model"> ���f�� </param>
	/// <param name="type"> �e�̎�� </param>
	/// <param name="iniPos"> �����ʒu </param>
	Bullet(Model* model, BulletType type, const float3& iniPos, const Vector3& moveVec);

	// �f�X�g���N�^
	~Bullet();

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

#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	/// <summary>
	/// �����t���O���擾
	/// </summary>
	/// <returns> �����t���O </returns>
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};