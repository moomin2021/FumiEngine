#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include <memory>

class Boss0
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	CollisionManager* colMgr_ = nullptr;

	// ���f��
	std::unique_ptr<Model> model_ = nullptr;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> object_ = nullptr;

	// �R���C�_�[
	std::unique_ptr<SphereCollider> collider_ = nullptr;

	// HP
	uint16_t hp_ = 20;

	uint16_t damageCounter_ = 0;

	bool isAlive_ = true;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Boss0();

	// �f�X�g���N�^
	~Boss0();

	// ����������
	void Initialize(const Vector3& pos);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �I�u�W�F�N�g�X�V
	void MatUpdate();

	// �Փˎ�����
	void OnCollision();
#pragma endregion

public:
	bool GetIsAlive() { return isAlive_; }
};