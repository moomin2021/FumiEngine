#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Object3D.h"
#include "SphereCollider.h"
#include "RayCollider.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "Bullet.h"
#include "Sprite.h"

#include <memory>
#include <deque>

class Player
{
	enum State {
		NORMAL,	// �ʏ���
		AIR,	// �󒆏��
		CLIMB,	// �o����
	};

	std::vector<std::string> stateName_ = {
		"NORMAL",
		"AIR",
		"CLIMB"
	};

#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	Key*	key_	= nullptr;// �L�[�{�[�h
	Mouse*	mouse_	= nullptr;// �}�E�X
	CollisionManager* colMgr_ = nullptr;// �R���W�����}�l�[�W���[
	EnemyManager* enemyMgr_ = nullptr;

	// ���_�J����
	std::unique_ptr<Camera> camera_ = nullptr;
	float fovAngleY_ = 70.0f;

	// ���f��
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Model> mSheriff_ = nullptr;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> object_ = nullptr;
	std::unique_ptr<Object3D> testObj_ = nullptr;
	std::unique_ptr<Object3D> oSheriff_ = nullptr;

	// �R���C�_�[
	std::unique_ptr<SphereCollider> playerCol_ = nullptr;// �v���C���[�̃R���C�_�[
	std::unique_ptr<RayCollider> legCol_ = nullptr;// �����̃R���C�_�[(���������Ɏg�p)
	std::unique_ptr<RayCollider> climbCol_ = nullptr;// �Ǔo��Ɏg���R���C�_�[
	std::unique_ptr<RayCollider> eyeCol_ = nullptr;// ���_�R���C�_�[

	// ���
	State state_ = NORMAL;

	// �J�������x
	float sencivity_ = 0.1f;

	// ���_�p�x
	Vector3 eyeAngle_ = { 0.0f, 90.0f, 0.0f };
	float diffusivity_ = 5.0f;
	const float maxDiffusivity_ = 5.0f;
	float adsRate_ = 0.0f;
	bool isAds_ = false;

	// �O���x�N�g��
	Vector3 forwardVec_ = { 0.0f, 0.0f, 0.0f };

	// �E�����x�N�g��
	Vector3 rightVec_ = { 0.0f, 0.0f, 0.0f };

	// �ړ����x�֘A
	float moveSpd_		= 0.0f;// �ړ����x
	float maxMoveSpd_	= 1.0f;// �ő�ړ����x
	float moveAcc_		= 0.1f;// �ړ������x

	// �N���X�w�A
	uint16_t crossHairHandle_ = 0;
	std::unique_ptr<Sprite> sCrossHair_ = nullptr;

	// �e
	std::deque<std::unique_ptr<Bullet>> bullets_;
	std::unique_ptr<Model> mBullet_ = nullptr;// �e�̃��f��
	uint8_t maxBullet_ = 30;// �ő�e��
	uint8_t nowBullet_ = 30;// ���ݒe��
	uint16_t bulletValueDisplayFrameHandle_ = 0;// �c�e���\��UI�n���h��
	std::unique_ptr<Sprite> sBulletValueDisplayFrame_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sMaxBulletUI_;// �ő�e���\���X�v���C�g
	std::vector<std::unique_ptr<Sprite>> sNowBulletUI_;// �c�e���\���X�v���C�g
	float shotInterval_ = 0.1f;// ���C���^�[�o��
	uint64_t shotTime_ = 0;// �e������������
	std::vector<uint16_t> numberHandle_;// ����

	// �����[�h
	bool isReload_ = false;	// �����[�h���Ă��邩
	uint8_t reloadTime_ = 3;// �����[�h����
	uint16_t reloadUIHandle_ = 0;// �����[�hUI�n���h��
	std::unique_ptr<Sprite> sReloadUI_ = nullptr;// �����[�hUI�X�v���C�g

	// �W�����v�p
	bool isGround_ = true;// �n�ʂɂ��Ă��邩
	float gravity_		= 0.0f;// �d��
	float maxGravity_	= 1.5f;// �ő�d��
	float gAcc_			= 0.2f;// �d�͉����x
	float jumpSpd_		= 1.5f;// �W�����v���x

	// �_�b�V��
	bool isDash_ = false;
	float dashSpd_ = 2.0f;
#pragma endregion

#pragma region �����o�֐�
public:
	// �f�X�g���N�^
	~Player();

	// ����������
	void Initialize(EnemyManager* enemyMgr);

	// �X�V����
	void Update();

	// �`�揈��
	void DrawObject3D();
	void DrawSprite();

	// �I�u�W�F�N�g�X�V����
	void ObjUpdate();

	// �Փˎ�����
	void OnCollision();
private:
	// ��ԕʏ���
	static void (Player::* stateTable[]) ();
	void Normal();	// �ʏ���
	void Air();		// �󒆏��
	void Climb();	// �o����

	// �s���֐�
	void Ads();		// �`������
	void Shoot();	// �e������
	void Reload();	// �����[�h����
	void Move();	// �ړ�����
	void EyeMove();	// ���_����
	void Jump();	// �W�����v����
	void Fall();	// ��������
	void Dash();	// ���s����
#pragma endregion
};