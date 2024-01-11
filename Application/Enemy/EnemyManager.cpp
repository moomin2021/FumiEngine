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
	colMgr_ = CollisionManager::GetInstance();

	Zombie::SetCollisionManager(colMgr_);
	Magician::SetCollisionManager(colMgr_);
	EnemyCore::SetCollisionManager(colMgr_);
	MagicianBullet::SetCollisionManager(colMgr_);
#pragma endregion

#pragma region モデル
	mBossGenerator_ = std::make_unique<Model>("bossGenerator");
	mEnemy0_ = std::make_unique<Model>("stoneGolem");
	mZombie_ = std::make_unique<Model>("zombie");
	mMagician_ = std::make_unique<Model>("magician");
	coreM_ = std::make_unique<Model>("core");
	coreFrameM_ = std::make_unique<Model>("coreFrame");
	coreStandM_ = std::make_unique<Model>("coreStand");

	Zombie::SetModel(mZombie_.get());
	Magician::SetModel(mMagician_.get());
	EnemyCore::SetModel(coreM_.get(), coreFrameM_.get(), coreStandM_.get());
#pragma endregion

#pragma region パーティクル
	particle_ = std::make_unique<ParticleEmitter>();
	deathParticleH_ = LoadTexture("Sprite/deathParticle.png");
	MagicianBullet::SetParticleHandle(LoadTexture("Sprite/enemyBulletParticle.png"));
#pragma endregion

	deltaTime_.Initialize();
}

void EnemyManager::Update()
{
	deltaTime_.Update();
	elapsedTime_ += deltaTime_.GetDeltaTime();

	for (auto& it : enemyGenerators_) it.Update();

	for (auto& it : enemyGenerators_)
	{
		if (it.GetSpawnFrag())
		{
			CreateAddEnemy0(it.GetSpawnPos());
		}
	}

	for (auto it = zombies_.begin(); it != zombies_.end();)
	{
		// 敵の更新
		(*it)->Update();

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false)
		{
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
			CreateAddEnemy0((*it)->GetSpawnPos());
		}

		// 敵の更新
		(*it)->Update();

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false)
		{
			it = enemyCores_.erase(it);
			breakCore_++;
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
}

void EnemyManager::MatUpdate()
{
	for (auto& it : zombies_) it->MatUpdate();
	for (auto& it : magicians_) it->MatUpdate();
	for (auto& it : enemyCores_) it->MatUpdate();

	particle_->Update(BILLBOARD::Y);
}

void EnemyManager::OnCollision()
{
	for (auto& it : zombies_) it->OnCollision();
	for (auto& it : magicians_) it->OnCollision();
	for (auto& it : enemyCores_) it->OnCollision();
}

void EnemyManager::CreateAddEnemy0(const Vector3& pos)
{
	uint16_t rnd = 0;
	rnd = Util::GetRandomInt(0, 10);

	if (rnd <= 7)
	{
		std::unique_ptr<Zombie> newEnemy = std::make_unique<Zombie>();
		newEnemy->Initialize(pos);

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
	enemyGenerators_.back().AddSpawnNum(1);
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
		CreateAddEnemy0(enemyCreatePos);
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
		particle_->Add(particleLife_, inPos + offset, velocity,
			{ -velocity.x / particleLife_, -velocity.y / particleLife_, -velocity.z / particleLife_ },
			startParticleScale_, endParticleScale_);
	}
}

void EnemyManager::SetPlayer(Player* player)
{
	Zombie::SetPlayer(player);
	Magician::SetPlayer(player);
	MagicianBullet::SetPlayer(player);
	EnemyCore::SetPlayer(player);
}
