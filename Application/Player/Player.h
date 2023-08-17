#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Object3D.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Bullet.h"
#include "Sprite.h"

#include <memory>
#include <deque>

class Player
{
	enum State {
		NORMAL,	// �ʏ���
		AIR,	// �󒆏��
	};

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

	// ���
	State state_ = NORMAL;

	// �J�������x
	float sencivity_ = 0.1f;

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

	// �s���֐�
	void Shoot();
	void Reload();
	void Move();
	void EyeMove();
#pragma endregion
};