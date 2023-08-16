#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Object3D.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include <memory>

class Player
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	Key*	key_	= nullptr;// �L�[�{�[�h
	Mouse*	mouse_	= nullptr;// �}�E�X
	CollisionManager* colMgr_ = nullptr;// �R���W�����}�l�[�W���[

	// ���_�J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// ���f��
	std::unique_ptr<Model> model_ = nullptr;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> object_ = nullptr;

	// �R���C�_�[
	std::unique_ptr<SphereCollider> playerCol_ = nullptr;// �v���C���[�̃R���C�_�[

	// �ړ����x�֘A
	float moveSpd_		= 0.0f;// �ړ����x
	float maxMoveSpd_	= 1.0f;// �ő�ړ����x
	float moveAcc_		= 0.1f;// �ړ������x

	// �J�������x
	float sencivity_ = 0.1f;

	// �O���x�N�g��
	Vector3 forwardVec_ = { 0.0f, 0.0f, 0.0f };
	// �E�����x�N�g��
	Vector3 rightVec_ = { 0.0f, 0.0f, 0.0f };
#pragma endregion

#pragma region �����o�֐�
public:
	// �f�X�g���N�^
	~Player();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �I�u�W�F�N�g�X�V����
	void ObjUpdate();

	// �Փˎ�����
	void OnCollision();
private:
	void Move();
	void EyeMove();
#pragma endregion
};