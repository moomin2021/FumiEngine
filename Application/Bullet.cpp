#include "Bullet.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

Bullet::Bullet(Model* model, BulletType type, const float3& iniPos, const Vector3& moveVec)
{
	// �I�u�W�F�N�g����������
	data_.object = std::make_unique<Object3D>(model);

	// �������ꂽ���Ԃ��L�^
	generatedTime_ = Util::GetTimeSec();

	// �v���C���[
	if (type == PLAYER) {
		data_.object->SetPosition(iniPos);
		data_.object->SetScale({ 0.1f, 0.1f, 0.1f });
		data_.moveVec = moveVec;
		data_.bulletSpd = 30.0f;
		data_.aliveTime = 3;
	}

	else if (type == ENEMY0) {
		data_.object->SetPosition(iniPos);
		data_.object->SetScale({ 0.5f, 0.5f, 0.5f });
		data_.object->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		data_.col = std::make_unique<SphereCollider>();
		data_.col->SetAttribute(COL_ENEMY_BULLET);
		data_.col->SetRadius(0.5f);
		data_.col->LinkObject3D(data_.object.get());
		CollisionManager::GetInstance()->AddCollider(data_.col.get());
		data_.moveVec = moveVec;
		data_.bulletSpd = 2.0f;
		data_.aliveTime = 30;
	}

	data_.moveVec.normalize();
}

Bullet::~Bullet()
{
	CollisionManager::GetInstance()->RemoveCollider(data_.col.get());
}

void Bullet::Update()
{
	// �����t���O��[OFF]�Ȃ炱�̌�̏������΂�
	if (isAlive_ == false) return;

	// �e���ړ�������
	data_.object->SetPosition(data_.object->GetPosition() + data_.moveVec * data_.bulletSpd);

	// �I�u�W�F�N�g�̍X�V����
	data_.object->Update();

	// ��������Ă���̌o�ߎ���
	uint64_t elapsedTime = Util::GetTimeSec() - generatedTime_;

	// �o�ߎ��Ԃ��w��̎��Ԃ��߂����琶���t���O��[OFF]�ɂ���
	if (elapsedTime >= data_.aliveTime) {
		isAlive_ = false;
	}
}

void Bullet::Draw()
{
	// �I�u�W�F�N�g�̕`�揈��
	data_.object->Draw();
}