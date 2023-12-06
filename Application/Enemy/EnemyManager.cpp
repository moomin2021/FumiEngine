#include "EnemyManager.h"
#include "CollisionAttribute.h"
#include "SceneManager.h"
#include "Texture.h"
#include "SceneManager.h"
#include "WinAPI.h"

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

#pragma region ハンドル
	hParticle_ = LoadTexture("Sprite/effect1.png");
#pragma endregion

#pragma region ナビメッシュ
	navMesh_ = std::make_unique<NavMesh>();
	navMesh_->Initialize("navMesh");
	navMesh_->SetIsDraw(false);
	Enemy0::SetNavMesh(navMesh_.get());
	Zombie::SetNavMesh(navMesh_.get());
	cellsCenter_ = navMesh_->GetCellsCenter();
#pragma endregion

	enemyGenerators_.emplace_front();
	enemyGenerators_.front().SetOffset({ 0.0f, 2.0f, 0.0f });
	enemyGenerators_.front().AddSpawnNum(5);
}

void EnemyManager::Update()
{
	for (auto& it : enemyGenerators_) it.Update();

	for (auto& it : enemyGenerators_)
	{
		if (it.GetSpawnFrag())
		{
			CreateAddEnemy0(it.GetSpawnPos());
		}
	}

	for (auto it = enemys_.begin(); it != enemys_.end();) {
		// 敵の更新
		(*it)->Update();

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false) it = enemys_.erase(it);
		else ++it;
	}

	for (auto it = zombies_.begin(); it != zombies_.end();)
	{
		// 敵の更新
		(*it)->Update();

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false)
		{
			it = zombies_.erase(it);
			enemyDeathCounter_++;
		}
		else ++it;
	}

	// コア
	for (auto it = enemyCores_.begin(); it != enemyCores_.end();)
	{
		// 敵の更新
		(*it)->Update();

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false) it = enemyCores_.erase(it);
		else ++it;
	}
}

void EnemyManager::Draw()
{

	// 敵描画処理
	for (auto& i : enemys_) {
		i->Draw();
	}

	for (auto& it : zombies_) it->Draw();

	for (auto& it : enemyCores_) it->Draw();

	navMesh_->Draw();
}

void EnemyManager::MatUpdate()
{

	for (auto& it : enemys_) it->MatUpdate();
	for (auto& it : zombies_) it->MatUpdate();

	navMesh_->MatUpdate();

	for (auto& it : enemyCores_) it->MatUpdate();
}

void EnemyManager::OnCollision()
{
	for (auto& it : enemys_) it->OnCollision();
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
}

void EnemyManager::CheckSceneChange()
{
	if (enemyCores_.empty())
	{
		SceneManager::GetInstance()->SceneTransition(SCENE::TITLE);
		WinAPI::GetInstance()->DisplayCursor(true);
		WinAPI::GetInstance()->SetClipCursor(false);
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
		DeleteEnemy0();
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
			for (auto& it : enemys_)
			{
				it->CreateNavRoute();
			}
		}

		ImGui::TreePop();
	}

	ImGui::End();
}

void EnemyManager::SetPlayer(Player* player)
{
	Enemy0::SetPlayer(player);
	Zombie::SetPlayer(player);
}
