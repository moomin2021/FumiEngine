#include "Enemy.h"

Model* Enemy::model_ = nullptr;

Enemy::Enemy() :
#pragma region �����o�ϐ�
	// �I�u�W�F�N�g
	obj_(nullptr),

	// �Փ˔���p
	col_{},

	// HP
	hp_(3),

	// �����t���O
	isAlive_(true)
#pragma endregion
{
	obj_ = std::make_unique<Object3D>(model_);
}

void Enemy::Update()
{
	// �����t���O��[OFF]�Ȃ炱�̌�̏������΂�
	if (isAlive_ == false) return;

	col_.pos = obj_->GetPosition();
	col_.radius = obj_->GetScale().x;

	// HP��0�ȉ��ɂȂ����琶���t���O��[OFF]�ɂ���
	if (hp_ <= 0) isAlive_ = false;
}

void Enemy::Draw()
{
	obj_->Draw();
}