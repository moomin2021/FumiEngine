#include "Bullet.h"

// �e�̃��f��
Model* Bullet::mBullet_ = nullptr;

Bullet::Bullet(const float3& iniPos, const Vector3& moveVec) :
#pragma region ���������X�g
	// �e�̃I�u�W�F�N�g
	oBullet_(nullptr),

	// �Փ˔���p
	//col_{},

	// �e�̈ړ�����
	moveVec_(moveVec),

	// �e�̑��x
	bulletSpd_(20.0f),

	// �����t���O
	isAlive_(true),

	// ��������
	aliveCounter_(0),	// ��������̃J�E���g
	maxCount_(300)		// �ő�J�E���g
#pragma endregion
{
	// �I�u�W�F�N�g�������ݒ�
	oBullet_ = std::make_unique<Object3D>(mBullet_);
	oBullet_->SetPosition(iniPos);				// �������W�̐ݒ�
	oBullet_->SetScale({ 0.1f, 0.1f, 0.1f });	// �傫���ݒ�

	// �ړ������𐳋K��
	moveVec_.normalize();
}

void Bullet::Update()
{
	// �����t���O��[OFF]�Ȃ炱�̌�̏������΂�
	if (isAlive_ == false) return;

	//col_.pos = oBullet_->GetPosition();
	//col_.radius = oBullet_->GetScale().x;

	// �e���ړ�������
	oBullet_->SetPosition(oBullet_->GetPosition() + moveVec_ * bulletSpd_);
	
	oBullet_->Update();

	// �J�E���g��i�߂�
	aliveCounter_++;

	// �����J�E���g���ő�J�E���g�𒴂����琶���t���O��[OFF]�ɂ���
	if (aliveCounter_ >= maxCount_) isAlive_ = false;
}

void Bullet::Draw()
{
	oBullet_->Draw();
}