#include "Enemy0.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

Enemy0::Enemy0(Model* model) :
	// HP
	hp_(3),

	damageCounter_(0)
{
	// �I�u�W�F�N�g����
	object_ = std::make_unique<Object3D>(model);
}

Enemy0::~Enemy0()
{
	// �R���C�_�[���폜
	CollisionManager::GetInstance()->RemoveCollider(collider_.get());
}

void Enemy0::Initialize(float3 pos, float3 scale)
{
	// �����ʒu�ݒ�
	object_->SetPosition(pos);
	object_->SetScale(scale);

	// �R���C�_�[����
	collider_ = std::make_unique<SphereCollider>(float3{0.0f, 0.0f, 0.0f}, 2.0f);

	// �R���C�_�[�ƃI�u�W�F�N�g��R�Â�
	collider_->LinkObject3D(object_.get());

	// �R���C�_�[�ɑ�����ǉ�
	collider_->SetAttribute(COL_ENEMY);

	// �R���C�_�[��ǉ�
	CollisionManager::GetInstance()->AddCollider(collider_.get());
}

void Enemy0::Update()
{
	// �����t���O��[OFF]�Ȃ炱�̌�̏������΂�
	if (isAlive_ == false) return;

	if (damageCounter_ >= 3) {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
	else {
		damageCounter_++;
	}

	// HP��0�ȉ��ɂȂ����琶���t���O��[OFF]�ɂ���
	if (hp_ <= 0) isAlive_ = false;

	// �Փˎ��̏���
	OnCollision();

	// �I�u�W�F�N�g�X�V
	object_->Update();
}

void Enemy0::Draw()
{
	// �I�u�W�F�N�g�`��
	if (isAlive_) object_->Draw();
}

void Enemy0::OnCollision()
{
	// �Փ˂��Ă��Ȃ������珈�����΂�
	if (collider_->GetIsHit() == false) return;
	hp_ -= 1;
	object_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
	damageCounter_ = 0;
}