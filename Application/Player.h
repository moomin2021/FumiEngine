#pragma once
#include "Key.h"
#include "Mouse.h"
#include "CollisionManager.h"

#include "Camera.h"

#include <string>
#include <vector>
#include <memory>

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

	// ���
	State state_ = NORMAL;

	// �J�����֘A
	std::unique_ptr<Camera> camera_ = nullptr;// �J�����{��
	float sencivity_ = 0.1f;// �J�������x
	Vector3 eyeAngle_	= { 0.0f, 0.0f, 0.0f };// �J�����p�x
	Vector3 forwardVec_	= { 0.0f, 0.0f, 0.0f };// ���ʃx�N�g��
	Vector3 rightVec_	= { 0.0f, 0.0f, 0.0f };// �E�x�N�g��

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
	void EyeMove();	// ���_����
#pragma endregion
};