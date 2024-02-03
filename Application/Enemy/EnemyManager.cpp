#include "EnemyManager.h"
#include "CollisionAttribute.h"
#include "SceneManager.h"
#include "Texture.h"
#include "SceneManager.h"
#include "WinAPI.h"
#include "PipelineManager.h"
#include "MagicianBullet.h"

#include <imgui_impl_DX12.h>

EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager()
{
	
}

void EnemyManager::Initialize()
{
#pragma region インスタンス
	colMgr_ = CollisionManager3D::GetInstance();

	Zombie::SetCollisionManager(colMgr_);
	Magician::SetCollisionManager(colMgr_);
	EnemyCore::SetCollisionManager(colMgr_);
	MagicianBullet::SetCollisionManager(colMgr_);
#pragma endregion

#pragma region モデル
	mBossGenerator_ = std::make_unique<Model>("bossGenerator");
	mEnemy0_ = std::make_unique<Model>("stoneGolem");
	mZombie0_ = std::make_unique<Model>("zombie0");
	mZombie1_ = std::make_unique<Model>("zombie1");
	mMagician_ = std::make_unique<Model>("magician");
	coreM_ = std::make_unique<Model>("core");
	coreFrameM_ = std::make_unique<Model>("coreFrame");
	coreStandM_ = std::make_unique<Model>("coreStand");

	Zombie::SetModel(mZombie0_.get(), mZombie1_.get());
	Magician::SetModel(mMagician_.get());
	EnemyCore::SetModel(coreM_.get(), coreFrameM_.get(), coreStandM_.get());
#pragma endregion

#pragma region パーティクル
	particle_ = std::make_unique<ParticleEmitter>();
	deathParticleH_ = LoadTexture("Sprite/deathParticle.png");
	MagicianBullet::SetParticleHandle(LoadTexture("Sprite/enemyBulletParticle.png"));

	headP0_ = std::make_unique<ParticleEmitter>();
	headP1_ = std::make_unique<ParticleEmitter>();
	headH_ = LoadTexture("Sprite/dot.png");
#pragma endregion

	deltaTime_.Initialize();
	rushT_.coolTime = 20.0f;
}

void EnemyManager::Update()
{
	deltaTime_.Update();
	elapsedTime_ += deltaTime_.GetDeltaTime();

	for (auto& it : enemyGenerators_) it.Update();

	for (auto it = zombies_.begin(); it != zombies_.end();)
	{
		// 敵の更新
		(*it)->Update();

		if ((*it)->GetIsBody())
		{
			ZombieRangeSearch((*it)->GetPosition());
		}

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false)
		{
			if ((*it)->GetIsHead())
			{
				CreateHeadP((*it)->GetPosition());
			}

			AddDeathParticle((*it)->GetPosition());
			it = zombies_.erase(it);
			enemyDeathCounter_++;
			killEnemy_++;
		}
		else ++it;
	}

	for (auto it = magicians_.begin(); it != magicians_.end();)
	{
		// 敵の更新
		(*it)->Update();

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false)
		{
			AddDeathParticle((*it)->GetPosition());
			it = magicians_.erase(it);
			enemyDeathCounter_++;
			killEnemy_++;
		}
		else ++it;
	}

	// コア
	for (auto it = enemyCores_.begin(); it != enemyCores_.end();)
	{
		if ((*it)->GetIsSpawn())
		{
			if (rushT_.GetOn()) CreateAddEnemy0((*it)->GetSpawnPos(), (*it)->GetPosition());
		}

		// 敵の更新
		(*it)->Update();

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false)
		{
			it = enemyCores_.erase(it);
			breakCore_++;
			rushT_.last = Util::GetTimrMSec();
			ZombieAllRushMode();
		}
		else ++it;
	}
}

void EnemyManager::Draw()
{
	PipelineManager::PreDraw("Object3D");
	for (auto& it : zombies_) it->Draw();
	for (auto& it : magicians_) it->Draw();
	PipelineManager::PreDraw("Object3D");
	for (auto& it : enemyCores_) it->Draw();

	PipelineManager::PreDraw("Particle", D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	particle_->Draw(deathParticleH_);
	headP0_->Draw(headH_);
	headP1_->Draw(headH_);
}

void EnemyManager::MatUpdate()
{
	for (auto& it : zombies_) it->MatUpdate();
	for (auto& it : magicians_) it->MatUpdate();
	for (auto& it : enemyCores_) it->MatUpdate();

	particle_->Update(BILLBOARD::Y);
	headP0_->Update(BILLBOARD::Y);
	headP1_->Update(BILLBOARD::Y);
}

void EnemyManager::OnCollision()
{
	for (auto& it : zombies_) it->OnCollision();
	for (auto& it : magicians_) it->OnCollision();
	for (auto& it : enemyCores_) it->OnCollision();
}

void EnemyManager::CreateAddEnemy0(const Vector3& pos, const Vector3& offset)
{
	uint16_t rnd = 0;
	rnd = Util::GetRandomInt(0, 5);

	if (rnd <= 7)
	{
		std::unique_ptr<Zombie> newEnemy = std::make_unique<Zombie>();
		if (rushCoolT_.GetOn() == false) newEnemy->Initialize(pos, offset, Zombie::State::CHASE);
		else newEnemy->Initialize(pos, offset, Zombie::State::WAIT);

		zombies_.emplace_back(std::move(newEnemy));
	}

	else
	{
		// 敵の生成
		std::unique_ptr<Magician> newEnemy = std::make_unique<Magician>();
		newEnemy->Initialize(pos);

		// エネミー配列に追加
		magicians_.emplace_back(std::move(newEnemy));
	}
}

void EnemyManager::AddCore(const Vector3& inPos)
{
	// 敵の生成
	std::unique_ptr<EnemyCore> newCore = std::make_unique<EnemyCore>();
	newCore->Initialize(inPos);

	// エネミー配列に追加
	enemyCores_.emplace_back(std::move(newCore));

	enemyGenerators_.emplace_back();
	enemyGenerators_.back().SetOffset(inPos);
	enemyGenerators_.back().AddSpawnNum(10);

	for (size_t i = 0; i < 10; i++)
	{
		CreateAddEnemy0(inPos, inPos);
	}
}

void EnemyManager::CheckSceneChange()
{
	if (enemyCores_.empty())
	{
		SceneManager::GetInstance()->SceneTransition(SCENE::GAMEOVER);
		WinAPI::GetInstance()->DisplayCursor(true);
		WinAPI::GetInstance()->SetClipCursor(false);

		ResultData* resultData_ = ResultData::GetInstance();
		resultData_->elapsedTime_ = (uint16_t)elapsedTime_;
		resultData_->killEnemy_ = killEnemy_;
		resultData_->breakCore_ = breakCore_;
		resultData_->isWin_ = true;
	}
}

void EnemyManager::Debug(bool isDebug)
{
	for (auto& it : zombies_) it->Debug(isDebug);
	for (auto& it : magicians_) it->Debug(isDebug);
	if (isDebug) for (auto& it : enemyCores_) it->Debug();

	if (isDebug == false) return;
	static Vector3 enemyCreatePos = { 0.0f, 0.0f, 0.0f };

	ImGui::Begin("Enemy");
	ImGui::SliderFloat("CreateEnemyPosX", &enemyCreatePos.x, -100.0f, 100.0f);
	ImGui::SliderFloat("CreateEnemyPosY", &enemyCreatePos.y, -100.0f, 100.0f);
	ImGui::SliderFloat("CreateEnemyPosZ", &enemyCreatePos.z, -100.0f, 100.0f);
	if (ImGui::Button("CreateEnemy"))
	{
		CreateAddEnemy0(enemyCreatePos, enemyCreatePos);
	}

	if (ImGui::Button("DeleteEnemy"))
	{

	}

	if (ImGui::TreeNode("NavMesh"))
	{
		static bool isMeshDraw = false;
		static bool isLinkLineDraw = false;
		ImGui::Checkbox("DrawMesh", &isMeshDraw);
		ImGui::Checkbox("DrawLinkLine", &isLinkLineDraw);

		if (ImGui::Button("CreateRoute"))
		{

		}

		ImGui::TreePop();
	}

	ImGui::End();
}

void EnemyManager::AddDeathParticle(const Vector3& inPos)
{
	for (size_t i = 0; i < particleNum_; i++)
	{
		Vector3 offset;
		offset.x = Util::GetRandomFloat(particleRnd_.x, particleRnd_.y);
		offset.y = Util::GetRandomFloat(particleRnd_.x, particleRnd_.y);
		offset.z = Util::GetRandomFloat(particleRnd_.x, particleRnd_.y);

		Vector3 velocity = { 0.0f, 1.0f, 0.0f };
		velocity.x = Util::GetRandomFloat(particleRnd_.x, particleRnd_.y);
		velocity.z = Util::GetRandomFloat(particleRnd_.x, particleRnd_.y);
		velocity.normalize();
		velocity /= (float)particleLife_;
		particle_->Add(particleLife_, inPos + offset + Vector3(0.0f, 1.5f, 0.0f), velocity,
			{ -velocity.x / particleLife_, -velocity.y / particleLife_, -velocity.z / particleLife_ },
			startParticleScale_, endParticleScale_);
	}
}

void EnemyManager::CreateHeadP(const Vector3& inPos)
{
	for (size_t i = 0; i < 50; i++)
	{
		Vector3 dir = Vector3();

		dir.x = Util::GetRandomFloat(-0.1f, 0.1f);
		dir.y = Util::GetRandomFloat(0.5f, 0.8f);
		dir.z = Util::GetRandomFloat(-0.1f, 0.1f);

		headP0_->Add(10, inPos + Vector3(0.0f, 1.75f, 0.0f), dir, -dir / 10.0f, 1.0f / 16.0f, 1.0f / 16.0f);
	}

	for (size_t i = 0; i < 25; i++)
	{
		Vector3 dir = Vector3();

		dir.x = Util::GetRandomFloat(-0.2f, 0.2f);
		dir.y = Util::GetRandomFloat(0.1f, 0.4f);
		dir.z = Util::GetRandomFloat(-0.2f, 0.2f);

		headP1_->Add(10, inPos + Vector3(0.0f, 1.75f, 0.0f), dir, -dir / 10.0f, 1.0f / 16.0f, 1.0f / 16.0f);
	}
}

void EnemyManager::ZombieAllRushMode()
{
	for (auto& it : zombies_)
	{
		it->SetRushMode();
	}
}

void EnemyManager::ZombieRangeSearch(const Vector3& inPos)
{
	for (auto& it : zombies_)
	{
		Vector3 v = inPos - it->GetPosition();
		if (v.length() <= 10.0f) it->SetRushMode();
	}
}

void EnemyManager::SetPlayer(Player* player)
{
	Zombie::SetPlayer(player);
	Magician::SetPlayer(player);
	MagicianBullet::SetPlayer(player);
	EnemyCore::SetPlayer(player);
}