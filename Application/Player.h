#pragma once
#include "Key.h"
#include "Mouse.h"
#include "CollisionManager.h"
#include "RayCollider.h"
#include "SphereCollider.h"
#include "Camera.h"
#include "Sprite.h"
#include "Sound.h"

#include "Bullet.h"
#include "ItemManager.h"

#include <string>
#include <vector>
#include <memory>
#include <deque>

class EnemyManager;

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
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;
	CollisionManager* colMgr_ = nullptr;
	EnemyManager* enemyMgr_ = nullptr;
	Sound* sound_ = nullptr;

	// ���
	State state_ = NORMAL;

	// �T�E���h
	uint16_t shotSE_ = 0;

	// �J�����֘A
	std::unique_ptr<Camera> camera_ = nullptr;// �J�����{��
	float sencivity_ = 0.1f;// �J�������x
	Vector3 eyeAngle_	= { 0.0f, 0.0f, 0.0f };// �J�����p�x
	Vector3 forwardVec_	= { 0.0f, 0.0f, 0.0f };// ���ʃx�N�g��
	Vector3 rightVec_	= { 0.0f, 0.0f, 0.0f };// �E�x�N�g��
	float fovAngleY_ = 70.0f;
	float diffusivity_ = 5.0f;
	const float maxDiffusivity_ = 5.0f;
	float adsRate_ = 0.0f;
	bool isAds_ = false;
	float recoilEyeAngle_ = 0.0f;

	// ���f��
	std::unique_ptr<Model> mSphere_ = nullptr;// ��
	std::unique_ptr<Model> mSheriff_ = nullptr;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> oPlayer_ = nullptr;// �v���C���[
	std::unique_ptr<Object3D> oSheriff_ = nullptr;

	// �R���C�_�[
	std::unique_ptr<SphereCollider> playerCol_ = nullptr;// �v���C���[�R���C�_�[
	std::unique_ptr<RayCollider> legCol_ = nullptr;// �����R���C�_�[
	std::unique_ptr<RayCollider> climbCol_ = nullptr;// �Ǔo��Ɏg���R���C�_�[
	std::unique_ptr<RayCollider> shotCol_ = nullptr;// �e�����������Ɏg���R���C�_�[
	std::unique_ptr<RayCollider> eyeCol_ = nullptr;// �����R���C�_�[

	// �ړ��֘A
	float moveSpd_ = 0.0f;// �ړ����x
	float maxMoveSpd_ = 0.5f;// �ő�ړ����x
	float moveAcc_ = 0.05f;// �ړ������x
	bool isDash_ = false;// �_�b�V���t���O
	float dashSpd_ = 1.0f;// �_�b�V�����x

	// �W�����v�֘A
	float gravity_ = 0.0f;// �d��
	float maxGravity_ = 1.5f;// �ő�d��
	float gAcc_ = 0.2f;// �d�͉����x
	float jumpSpd_ = 1.5f;// �W�����v���x

	// �N���X�w�A
	uint16_t crossHairHandle_ = 0;
	std::unique_ptr<Sprite> sCrossHair_ = nullptr;

	// �e
	std::unique_ptr<Model> mBullet_ = nullptr;
	std::deque<std::unique_ptr<Bullet>> bullets_ = {};
	uint8_t maxBullet_ = 99;// �ő�e��
	uint8_t nowBullet_ = 99;// ���ݒe��
	uint16_t bulletValueDisplayFrameHandle_ = 0;// �c�e���\��UI�t���[���n���h��
	std::vector<uint16_t> numberHandle_ = {};// �����n���h��
	std::unique_ptr<Sprite> sBulletValueDisplayFrame_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sMaxBulletUI_ = {};// �ő�e���\���X�v���C�g
	std::vector<std::unique_ptr<Sprite>> sNowBulletUI_ = {};// �c�e���\���X�v���C�g
	float shotInterval_ = 0.7f;
	uint64_t shotTime_ = 0;

	// �����[�h
	bool isReload_ = false;	// �����[�h���Ă��邩
	uint8_t reloadTime_ = 3;// �����[�h����
	uint16_t reloadUIHandle_ = 0;// �����[�hUI�n���h��
	std::unique_ptr<Sprite> sReloadUI_ = nullptr;// �����[�hUI�X�v���C�g

	// �A�C�e��
	std::vector<uint8_t> items_;
	ItemManager* itemManager_ = nullptr;

	// ����q���g
	std::unique_ptr<Sprite> opeTips_ = nullptr;
	uint16_t opeTipsHandle_ = 0;
	bool isHitItem_ = false;
	bool isBossGen_ = false;

#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// 3D�`�揈��
	void Draw3D();

	// �O��2D�`�揈��
	void DrawFront2D();

	// �Փ˔��莞����
	void OnCollision();

	// �s��X�V����
	void MatUpdate();

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

#pragma region �Z�b�^�[�֐�
	public:
	void SetEnemyManager(EnemyManager* inst) { enemyMgr_ = inst; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	inline const Vector3& GetPosition() { return camera_->GetEye(); }
#pragma endregion
};