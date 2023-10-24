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
	colMgr_->RemoveCollider(colBossGenerator_.get());
}

void EnemyManager::Initialize()
{
#pragma region インスタンス
	colMgr_ = CollisionManager::GetInstance();
#pragma endregion

#pragma region モデル
	mBossGenerator_ = std::make_unique<Model>("bossGenerator");
	mEnemy0_ = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region オブジェクト
	oBossGenerator_ = std::make_unique<Object3D>(mBossGenerator_.get());
#pragma endregion

#pragma region コライダー
	colBossGenerator_ = std::make_unique<SphereCollider>(Vector3{ 0.0f, 3.0f, 0.0f });
	colBossGenerator_->SetAttribute(COL_BOSSGENERATOR);
	colBossGenerator_->SetObject3D(oBossGenerator_.get());
	colMgr_->AddCollider(colBossGenerator_.get());
#pragma endregion

#pragma region ハンドル
	hParticle_ = LoadTexture("Resources/effect1.png");
#pragma endregion

#pragma region ナビメッシュ
	navMesh_ = std::make_unique<NavMesh>();
	navMesh_->Initialize("navMesh");
	navMesh_->SetIsDraw(false);
	Enemy0::SetNavMesh(navMesh_.get());
#pragma endregion
}

void EnemyManager::Update()
{
	for (auto it = enemys_.begin(); it != enemys_.end();) {
		// 敵の更新
		(*it)->Update();

		// 敵の生存フラグが[OFF]になったら消す
		if ((*it)->GetIsAlive() == false) it = enemys_.erase(it);
		else ++it;
	}

	// ボスが生成されていたら処理をする
	if (boss_) {
		boss_->Update();
	}
}

void EnemyManager::Draw()
{
	oBossGenerator_->Draw();
	if (boss_) {
		boss_->Draw();
	}

	// 敵描画処理
	for (auto& i : enemys_) {
		i->Draw();
	}

	navMesh_->Draw();
}

void EnemyManager::MatUpdate()
{
	oBossGenerator_->MatUpdate();
	if (boss_) {
		boss_->MatUpdate();
	}

	for (auto& it : enemys_) it->MatUpdate();

	navMesh_->MatUpdate();
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
	// 敵の生成
	std::unique_ptr<Enemy0> newEnemy = std::make_unique<Enemy0>(mEnemy0_.get());
	newEnemy->Initialize(pos, scale);

	// エネミー配列に追加
	enemys_.emplace_back(std::move(newEnemy));
}

void EnemyManager::CheckSceneChange()
{
	if (boss_ && boss_->GetIsAlive() == false) {
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
	static Vector3 enemyCreatePos = { 0.0f, 1.0f, 0.0f };

	ImGui::Begin("Enemy");
	ImGui::SliderFloat("CreateEnemyPosX", &enemyCreatePos.x, -100.0f, 100.0f);
	ImGui::SliderFloat("CreateEnemyPosY", &enemyCreatePos.y, -100.0f, 100.0f);
	ImGui::SliderFloat("CreateEnemyPosZ", &enemyCreatePos.z, -100.0f, 100.0f);
	if (ImGui::Button("CreateEnemy"))
	{
		CreateAddEnemy0(enemyCreatePos, Vector3{ 1.0f, 1.0f, 1.0f });
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

void EnemyManager::SetBossGenerator(const Vector3& pos)
{
	oBossGenerator_->SetPosition(pos);
}

void EnemyManager::SetPlayer(Player* player)
{
	Enemy0::SetPlayer(player);
}
