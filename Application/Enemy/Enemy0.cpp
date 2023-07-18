#include "Enemy0.h"
#include "CollisionManager.h"

Enemy0::Enemy0(Model* model)
{
	// �I�u�W�F�N�g����
	object_ = std::make_unique<Object3D>(model);
}

Enemy0::~Enemy0()
{
	// �R���C�_�[���폜
	CollisionManager::GetInstance()->RemoveCollider(collider_.get());
}

void Enemy0::Initialize(float3 pos)
{
	// �����ʒu�ݒ�
	object_->SetPosition(pos);

	// �R���C�_�[����
	collider_ = std::make_unique<SphereCollider>(pos);

	// �R���C�_�[�ƃI�u�W�F�N�g��R�Â�
	collider_->LinkObject3D(object_.get());

	// �R���C�_�[��ǉ�
	CollisionManager::GetInstance()->AddCollider(collider_.get());
}

void Enemy0::Update()
{
	// �I�u�W�F�N�g�X�V
	object_->Update();
}

void Enemy0::Draw()
{
	// �I�u�W�F�N�g�`��
	object_->Draw();
}