#include "Enemy0.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "EnemyManager.h"

Player* Enemy0::player_ = nullptr;

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

	// ��ԕʍX�V����
	(this->*stateTable[state_])();

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

void (Enemy0::* Enemy0::stateTable[]) () = {
	&Enemy0::Wait,		// �ҋ@���
	&Enemy0::RandomMove,// �����_���ړ����
	&Enemy0::Chase,		// �ǐՏ��
};

void Enemy0::Wait()
{
	// �o�ߎ���
	uint64_t elapsedTime = Util::GetTimeSec() - waitStartTime_;

	// �ҋ@���ԊJ�n���w�莞�Ԉȏ�Ȃ烉���_���ړ���Ԃɂ���
	if (waitStartTime_ <= elapsedTime) {
		state_ = RANDOMMOVE;
		randomMoveVec_ = Vector3(
			Util::GetRandomFloat(-1.0f, 1.0f), 0.0f,
			Util::GetRandomFloat(-1.0f, 1.0f)).normalize();
		rndMoveStartTime_ = Util::GetTimeSec();
	}

	// �v���C���[�Ƃ̋���
	float dist = Vector3(object_->GetPosition() - player_->GetPosition()).length();

	// �v���C���[�����G�͈͂ɓ�������ǐՏ�ԂɂȂ�
	if (searchRange_ >= dist) {
		state_ = CHASE;
		horizontalMoveStartTime_ = Util::GetTimeSec();
	}
}

void Enemy0::RandomMove()
{
	// �o�ߎ���
	uint64_t elapsedTime = Util::GetTimeSec() - rndMoveStartTime_;

	// ���W�X�V
	float3 pos = object_->GetPosition() + (randomMoveVec_ * rndMoveSpd_);
	object_->SetPosition(pos);

	// �o�ߎ��Ԃ��w��̎��Ԃ��������烉���_���ړ�����߂�
	if (rndMoveTime_ <= elapsedTime) {
		state_ = WAIT;
		waitStartTime_ = Util::GetTimeSec();
	}

	// �v���C���[�Ƃ̋���
	float dist = Vector3(object_->GetPosition() - player_->GetPosition()).length();

	// �v���C���[�����G�͈͂ɓ�������ǐՏ�ԂɂȂ�
	if (searchRange_ >= dist) {
		state_ = CHASE;
		horizontalMoveStartTime_ = Util::GetTimeSec();
	}
}

void Enemy0::Chase()
{
	// ���ړ��o�ߎ���
	uint64_t elapsedTime = Util::GetTimeSec() - horizontalMoveStartTime_;

	// �G�l�~�[����v���C���[�܂ł̕���
	float3 s = player_->GetPosition() - object_->GetPosition();
	Vector3 enemy2Player = Vector3(player_->GetPosition() - object_->GetPosition());
	enemy2Player.y = 0.0f;
	enemy2Player.normalize();

	// �E�x�N�g���̌v�Z
	Vector3 rightVec = Vector3(enemy2Player.x, 0.0f, enemy2Player.z);
	rightVec = -rightVec.cross(rightVec + Vector3(0.0f, 1.0f, 0.0f));
	rightVec.normalize();

	// ���ړ��o�ߎ��Ԃ��w�莞�Ԃ��߂����牡�ړ�������؂�ւ���
	if (horizontalMoveSwitchTime_ <= elapsedTime) {
		isMoveRight_ *= -1.0f;
		horizontalMoveStartTime_ = Util::GetTimeSec();
	}

	float3 pos = object_->GetPosition() + (enemy2Player * frontRearMoveSpd_) + (rightVec * horizontalMoveSpd_ * isMoveRight_);
	object_->SetPosition(pos);

	// �e��������
	Shoot();
}

void Enemy0::OnCollision()
{
	// �Փ˂��Ă��Ȃ������珈�����΂�
	if (collider_->GetIsHit() == false) return;
	hp_ -= 1;
	object_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
	damageCounter_ = 0;
}

void Enemy0::Shoot()
{
	//// �����Ă���̌o�ߎ���
	//uint64_t elapsedTime = Util::GetTimeSec() - shootTime_;

	//// �o�ߎ��Ԃ��w�莞�Ԃ��߂����猂��
	//if (elapsedTime >= shootInterval_) {
	//	// ���������Ԃ��L�^
	//	shootTime_ = Util::GetTimeSec();

	//	// �e�𐶐�
	//	EnemyManager::AddBullet(ENEMY0, object_->GetPosition(), player_->GetPosition() - object_->GetPosition());
	//}
}