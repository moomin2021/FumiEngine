#include "EnemyManager.h"
#include "CollisionAttribute.h"
#include "SceneManager.h"
#include "Texture.h"
#include "SceneManager.h"
#include "WinAPI.h"
#include "PipelineManager.h"

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
	EnemyCore::SetCollisionManager(colMgr_);
#pragma endregion

#pragma region モデル
	mBossGenerator_ = std::make_unique<Model>("bossGenerator");
	mEnemy0_ = std::make_unique<Model>("stoneGolem");
	mZombie_ = std::make_unique<Model>("zombie");
	coreM_ = std::make_unique<Model>("core");
	coreFrameM_ = std::make_unique<Model>("coreFrame");
	coreStandM_ = std::make_unique<Model>("coreStand");

	Zombie::SetModel(mZombie_.get());
	EnemyCore::SetModel(coreM_.get(), coreFrameM_.get(), coreStandM_.get());
#pragma endregion

#pragma region ナビメッシュ
	navMesh_ = std::make_unique<NavMesh>();
	navMesh_->Initialize("navMesh");
	navMesh_->SetIsDraw(false);
	Zombie::SetNavMesh(navMesh_.get());
	cellsCenter_ = navMesh_->GetCellsCenter();
#pragma endregion

#pragma region パーティクル
	particle_ = std::make_unique<ParticleEmitter>();
	deathParticleH_ = LoadTexture("Sprite/deathParticle.png");
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

	// コア
	for (auto it = enemyCores_.begin(); it != enemyCores_.end();)
	{
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
	for (auto& it : enemyCores_) it->Draw();
	navMesh_->Draw();

	PipelineManager::PreDraw("Particle", D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	particle_->Draw(deathParticleH_);
}

void EnemyManager::MatUpdate()
{
	for (auto& it : zombies_) it->MatUpdate();

	navMesh_->MatUpdate();

	for (auto& it : enemyCores_) it->MatUpdate();

	particle_->Update(BILLBOARD::Y);
}

void EnemyManager::OnCollision()
{
	for (auto& it : zombies_) it->OnCollision();

	for (auto& it : enemyCores_) it->OnCollision();
}

void EnemyManager::CreateAddEnemy0(const Vector3& pos)
{
	// 敵の生成
	std::unique_ptr<Zombie> newZombie = std::make_unique<Zombie>();
	newZombie->Initialize(pos);

	// エネミー配列に追加
	zombies_.emplace_back(std::move(newZombie));
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
	enemyGenerators_.back().AddSpawnNum(5);
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
	}
}

void EnemyManager::Debug()
{
	for (auto& it : zombies_) it->Debug();
	for (auto& it : enemyCores_) it->Debug();

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
		navMesh_->SetIsDraw(isMeshDraw);
		navMesh_->SetIsLinkLineDraw(isLinkLineDraw);

		if (ImGui::Button("CreateRoute"))
		{

		}

		ImGui::TreePop();
	}

	ImGui::End();
}

void EnemyManager::AddDeathParticle(const Vector3& inPos)
{
	for (size_t i = 0; i < 10; i++)
	{
		Vector3 offset;
		offset.x = Util::GetRandomFloat(-0.5f, 0.5f);
		offset.y = Util::GetRandomFloat(-0.5f, 0.5f);
		offset.z = Util::GetRandomFloat(-0.5f, 0.5f);

		Vector3 velocity = { 0.0f, 1.0f, 0.0f };
		velocity.x = Util::GetRandomFloat(-0.5f, 0.5f);
		velocity.z = Util::GetRandomFloat(-0.5f, 0.5f);
		velocity.normalize();
		velocity /= 60.0f;
		particle_->Add(60, inPos + offset, velocity, { -velocity.x / 60.0f, -velocity.y / 60.0f, -velocity.z / 60.0f }, 0.5f, 0.0f);
	}
}

void EnemyManager::SetPlayer(Player* player)
{
	Zombie::SetPlayer(player);
}
