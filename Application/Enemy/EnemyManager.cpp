#include "EnemyManager.h"
#include "CollisionAttribute.h"
#include "SceneManager.h"
#include "Texture.h"
#include "SceneManager.h"

EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager()
{
	colMgr_->RemoveCollider(colBossGenerator_.get());
}

void EnemyManager::Initialize()
{
#pragma region �C���X�^���X
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region ���f��
	mBossGenerator_ = std::make_unique<Model>("bossGenerator");
	mEnemy0_ = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region �I�u�W�F�N�g
	oBossGenerator_ = std::make_unique<Object3D>(mBossGenerator_.get());
#pragma endregion

#pragma region �R���C�_�[
	colBossGenerator_ = std::make_unique<SphereCollider>(Vector3{ 0.0f, 3.0f, 0.0f });
	colBossGenerator_->SetAttribute(COL_BOSSGENERATOR);
	colBossGenerator_->SetObject3D(oBossGenerator_.get());
	colMgr_->AddCollider(colBossGenerator_.get());
#pragma endregion

#pragma region �n���h��
	hParticle_ = LoadTexture("Resources/effect1.png");
#pragma endregion
}

void EnemyManager::Update()
{
	for (auto it = enemys_.begin(); it != enemys_.end();) {
		// �G�̍X�V
		(*it)->Update();

		// �G�̐����t���O��[OFF]�ɂȂ��������
		if ((*it)->GetIsAlive() == false) it = enemys_.erase(it);
		else ++it;
	}

	// �{�X����������Ă����珈��������
	if (boss_) {
		boss_->Update();

		if (boss_->GetIsAlive() == false) {
			SceneManager::GetInstance()->SceneTransition(TITLE);
		}
	}
}

void EnemyManager::Draw()
{
	//oBossGenerator_->Draw();
	//if (boss_) {
	//	boss_->Draw();
	//}

	// �G�`�揈��
	for (auto& i : enemys_) {
		i->Draw();
	}
}

void EnemyManager::MatUpdate()
{
	oBossGenerator_->MatUpdate();
	if (boss_) {
		boss_->MatUpdate();
	}

	for (auto& it : enemys_) it->MatUpdate();
}

void EnemyManager::OnCollision()
{
	if (boss_) boss_->OnCollision();

	for (auto& it : enemys_) it->OnCollision();
}

void EnemyManager::SummonBoss()
{
	boss_ = std::make_unique<Boss0>();
	boss_->Initialize(oBossGenerator_->GetPosition() + Vector3{0.0f, 3.0f, 0.0f});
	boss_->MatUpdate();
}

void EnemyManager::CreateAddEnemy0(const Vector3& pos, const Vector3& scale)
{
	// �G�̐���
	std::unique_ptr<Enemy0> newEnemy = std::make_unique<Enemy0>(mEnemy0_.get());
	newEnemy->Initialize(pos, scale);

	// �G�l�~�[�z��ɒǉ�
	enemys_.emplace_back(std::move(newEnemy));
}

void EnemyManager::CheckSceneChange()
{
	if (boss_ && boss_->GetIsAlive() == false) {
		SceneManager::GetInstance()->SceneTransition(SCENE::TITLE);
	}
}

void EnemyManager::AddParticle(const Vector3& pos)
{
	std::unique_ptr<ParticleEmitter> newParticle = std::make_unique<ParticleEmitter>();
	for (size_t i = 0; i < 20; i++) {
		Vector3 vel{};
		vel.x = Util::GetRandomFloat(-5.0f, 5.0f);
		vel.y = Util::GetRandomFloat(-5.0f, 5.0f);
		vel.z = Util::GetRandomFloat(-5.0f, 5.0f);

		Vector3 acc{};
		acc.x = 0.0f;
		acc.y = 0.0f;
		acc.z = 0.0f;

		newParticle->Add(10, { 0.0f, 0.0f, 0.0f }, vel, acc, 10.0f, 0.0f);
		newParticle->SetSpawnPos(pos);
	}

	particles_.emplace_back(std::move(newParticle));
}

void EnemyManager::SetBossGenerator(const Vector3& pos)
{
	oBossGenerator_->SetPosition(pos);
}

void EnemyManager::SetPlayer(Player* player)
{
	Enemy0::SetPlayer(player);
}