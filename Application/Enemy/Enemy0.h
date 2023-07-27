#pragma once
#include "BaseEnemy.h"
#include "Bullet.h"
#include "Object3D.h"
#include "SphereCollider.h"
#include "Player.h"

#include <memory>

class Enemy0 : public BaseEnemy
{
	enum State {
		WAIT,		// �ҋ@���
		RANDOMMOVE,	// �����_���ړ����
		CHASE,		// �ǐՏ��
	};

#pragma region �����o�ϐ�
private:
	// �v���C���[
	static Player* player_;

	// ���
	State state_ = WAIT;

	// �e
	std::vector<std::unique_ptr<Bullet>> bullets_;
	static Model* sBulletModel_;

	// �e��Ԃ̊J�n����
	uint64_t waitStartTime_ = 0;
	uint64_t rndMoveStartTime_ = 0;

	// �e��Ԃ̐�������
	uint16_t waitTime_ = 3;
	uint16_t rndMoveTime_ = 5;

	// ���ړ��؂�ւ�����
	uint16_t horizontalMoveSwitchTime_ = 3;

	// ���ړ��J�n����
	uint64_t horizontalMoveStartTime_ = 0;

	// ���ړ����x
	float horizontalMoveSpd_ = 0.3f;

	// �O��ړ����x
	float frontRearMoveSpd_ = 0.1f;

	// �E�ړ���
	float isMoveRight_ = 1.0f;

	// �����_���ړ����̈ړ�����
	Vector3 randomMoveVec_ = {};

	// ���G�͈�
	float searchRange_ = 100.0f;

	// �����_���ړ����̑��x
	float rndMoveSpd_ = 0.1f;

	// ���Ԋu
	uint16_t shootInterval_ = 3;

	// ����������
	uint64_t shootTime_ = 0;

	// HP
	uint16_t hp_;

	uint16_t damageCounter_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model"> ���f�� </param>
	Enemy0(Model* model);

	// �f�X�g���N�^
	~Enemy0();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos"> �����ʒu </param>
	void Initialize(float3 pos, float3 scale);

	// �X�V����
	void Update() override;

	// �`�揈��
	void Draw() override;

private:
	// ��ԕʏ���
	static void (Enemy0::* stateTable[]) ();
	void Wait();		// �ҋ@���
	void RandomMove();	// �����_���ړ����
	void Chase();		// �ǐՏ��

	// �e������
	void Shoot();

	// �Փˎ��̏���
	void OnCollision();
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	/// <summary>
	/// �v���C���[��ݒ�
	/// </summary>
	static void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// �e�̃��f����ݒ�
	/// </summary>
	static void SetBulletModel(Model* model) { sBulletModel_ = model; }
#pragma endregion
};