#include "EnemyManager.h"
#include "CollisionManager.h"
#include "Enemy0.h"
#include "PipelineManager.h"
#include "Texture.h"

std::unique_ptr<Model> EnemyManager::mEnemy0Bullet_ = nullptr;
std::vector<std::unique_ptr<Bullet>> EnemyManager::bullets_ = {};

EnemyManager::EnemyManager() {
	// ���f���ǂݍ���
	mEnemy0_ = std::make_unique<Model>("sphere");
	mEnemy0Bullet_ = std::make_unique<Model>("sphere");
	particleHandle_ = LoadTexture("Resources/effect1.png");
}

void EnemyManager::Update()
{
	// �G�̍X�V
	for (size_t i = 0; i < enemys_.size(); i++) {
		enemys_[i]->Update();

		if (enemys_[i]->GetIsAlive() == false) {
			AddParticle(enemys_[i]->GetPos());
			enemys_.erase(enemys_.begin() + i);
		}
	}

	for (auto& i : enemys_) {
		i->Update();
	}

	for (size_t i = 0; i < bullets_.size(); i++) {
		// �e�̍X�V����
		bullets_[i]->Update();

		// �����t���O��[OFF]��������
		if (bullets_[i]->GetIsAlive() == false) {
			// �e������
			bullets_.erase(bullets_.begin() + i);
		}
	}

	for (auto it = particles_.begin(); it != particles_.end();) {
		// �����t���O��[OFF]�Ȃ�
		if ((*it)->GetIsAlive() == false) {
			// �폜���ꂽ�v�f�̎����w���C�e���[�^���Ԃ����B
			it = particles_.erase(it);
		}
		// �v�f�폜�����Ȃ��ꍇ�ɁA�C�e���[�^��i�߂�
		else {
			++it;
		}
	}

	for (auto& i : particles_) {
		i->Update(BILLBOARD::ALL);
	}
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �G�`�揈��
	for (auto& i : enemys_) {
		i->Draw();
	}

	for (auto& i : bullets_) {
		i->Draw();
	}

	PipelineManager::PreDraw("Particle", D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	for (auto& i : particles_) {
		i->Draw(particleHandle_);
	}
}

void EnemyManager::CreateAddEnemy0(const float3& pos, const float3& scale)
{
	// �G�̐���
	std::unique_ptr<Enemy0> newEnemy = std::make_unique<Enemy0>(mEnemy0_.get());
	newEnemy->Initialize(pos, scale);

	// �G�l�~�[�z��ɒǉ�
	enemys_.emplace_back(std::move(newEnemy));
}

void EnemyManager::AddBullet(BulletType type, const float3& iniPos, const Vector3& moveVec)
{
	bullets_.emplace_back(std::make_unique<Bullet>(mEnemy0Bullet_.get(), ENEMY0, iniPos, moveVec));
}

void EnemyManager::AddParticle(const float3& pos) {
	std::unique_ptr<ParticleEmitter> newParticle = std::make_unique<ParticleEmitter>();
	for (size_t i = 0; i < 20; i++) {
		float3 vel{};
		vel.x = Util::GetRandomFloat(-5.0f, 5.0f);
		vel.y = Util::GetRandomFloat(-5.0f, 5.0f);
		vel.z = Util::GetRandomFloat(-5.0f, 5.0f);

		float3 acc{};
		acc.x = 0.0f;
		acc.y = 0.0f;
		acc.z = 0.0f;

		newParticle->Add(10, {0.0f, 0.0f, 0.0f}, vel, acc, 10.0f, 0.0f);
		newParticle->SetPos(pos);
	}

	particles_.emplace_back(std::move(newParticle));
}

void EnemyManager::SetPlayer(Player* player)
{
	Enemy0::SetPlayer(player);
}