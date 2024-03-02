#include "EnemyManager.h"
#include "CollisionAttribute.h"
#include "SceneManager.h"
#include "Texture.h"
#include "SceneManager.h"
#include "WinAPI.h"
#include "PipelineManager.h"
#include "MagicianBullet.h"
#include "CollisionAttribute.h"

#include <imgui_impl_DX12.h>


EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager()
{
	colMgr_->RemoveCollider(mouseRay_.get());
}

void EnemyManager::Initialize()
{
#pragma region インスタンス
	colMgr_ = CollisionManager3D::GetInstance();
	mouse_ = Mouse::GetInstance();

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

#pragma region コライダー
	mouseRay_ = std::make_unique<RayCollider>();
	mouseRay_->SetAttribute(0);
	colMgr_->AddCollider(mouseRay_.get());
	Zombie::SetDebugNavGoal(&debugGoal_);
#pragma endregion
}

void EnemyManager::Update()
{
	deltaTime_.Update();
	elapsedTime_ += deltaTime_.GetDeltaTime();

	for (auto& it : enemyGenerators_) it.Update();

	BoidCohesion();
	ZombieUpdate();

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

	uint8_t counter = 0;

	// コア
	for (auto it = enemyCores_.begin(); it != enemyCores_.end();)
	{
		if ((*it)->GetIsSpawn())
		{
			if (rushT_.GetOn()) CreateAddEnemy0((*it)->GetSpawnPos(), (*it)->GetPosition(), counter);
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

		counter++;
	}
}

void EnemyManager::Draw()
{
	PipelineManager::PreDraw("Object3D");
	for (auto& it : zombies_) it->Draw();
	for (auto& it : zombies0_) it->Draw();
	for (auto& it : zombies1_) it->Draw();
	for (auto& it : zombies2_) it->Draw();
	for (auto& it : zombies3_) it->Draw();
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
	for (auto& it : zombies0_) it->MatUpdate();
	for (auto& it : zombies1_) it->MatUpdate();
	for (auto& it : zombies2_) it->MatUpdate();
	for (auto& it : zombies3_) it->MatUpdate();
	for (auto& it : magicians_) it->MatUpdate();
	for (auto& it : enemyCores_) it->MatUpdate();

	particle_->Update(BILLBOARD::Y);
	headP0_->Update(BILLBOARD::Y);
	headP1_->Update(BILLBOARD::Y);
}

void EnemyManager::OnCollision()
{
	for (auto& it : zombies_) it->OnCollision();
	for (auto& it : zombies0_) it->OnCollision();
	for (auto& it : zombies1_) it->OnCollision();
	for (auto& it : zombies2_) it->OnCollision();
	for (auto& it : zombies3_) it->OnCollision();
	for (auto& it : magicians_) it->OnCollision();
	for (auto& it : enemyCores_) it->OnCollision();

	if (mouseRay_->GetIsHit())
	{
		//debugGoal_ = mouseRay_->GetInter() + Vector3(0.0f, 1.0f, 0.0f);
	}
}

//void EnemyManager::CreateAddEnemy0(const Vector3& pos, const Vector3& offset)
//{
//	uint16_t rnd = 0;
//	rnd = Util::GetRandomInt(0, 5);
//
//	if (rnd <= 7)
//	{
//		std::unique_ptr<Zombie> newEnemy = std::make_unique<Zombie>();
//		if (rushCoolT_.GetOn() == false) newEnemy->Initialize(pos, offset, Zombie::State::CHASE);
//		else newEnemy->Initialize(pos, offset, Zombie::State::WAIT);
//
//		zombies_.emplace_back(std::move(newEnemy));
//	}
//
//	else
//	{
//		// 敵の生成
//		std::unique_ptr<Magician> newEnemy = std::make_unique<Magician>();
//		newEnemy->Initialize(pos);
//
//		// エネミー配列に追加
//		magicians_.emplace_back(std::move(newEnemy));
//	}
//}

void EnemyManager::CreateAddEnemy0(const Vector3& pos, const Vector3& offset, uint8_t id)
{
	std::unique_ptr<Zombie> newEnemy = std::make_unique<Zombie>();
	if (rushCoolT_.GetOn() == false) newEnemy->Initialize(pos, offset, Zombie::State::CHASE);
	else newEnemy->Initialize(pos, offset, Zombie::State::WAIT);

	if (id == 0) zombies0_.emplace_back(std::move(newEnemy));
	if (id == 1) zombies1_.emplace_back(std::move(newEnemy));
	if (id == 2) zombies2_.emplace_back(std::move(newEnemy));
	if (id == 3) zombies3_.emplace_back(std::move(newEnemy));
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
		CreateAddEnemy0(inPos, inPos, coreCounter_);
	}

	coreCounter_++;
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
	for (auto& it : zombies0_) it->Debug(isDebug);
	for (auto& it : zombies1_) it->Debug(isDebug);
	for (auto& it : zombies2_) it->Debug(isDebug);
	for (auto& it : zombies3_) it->Debug(isDebug);
	if (isDebug) for (auto& it : enemyCores_) it->Debug();

	if (isDebug == false) return;
	static Vector3 enemyCreatePos = { 0.0f, 0.0f, 0.0f };

#pragma region 視線先

	if (mouse_->TriggerMouseButton(MouseButton::M_LEFT))
	{
		Vector3 start = Vector3();
		Vector3 end = Vector3();
		Vector3 mousePos = { mouse_->MousePos().x, mouse_->MousePos().y, 0.0f };

		Matrix4 invView, invPrj, vp, invViewport;
		invView = Matrix4Inverse(debugCamera_->GetMatView());
		invPrj = Matrix4Inverse(debugCamera_->GetMatProjection());
		vp = Matrix4Identity();
		vp.m[0][0] = 1920.0f / 2.0f, vp.m[1][1] = -1080.0f / 2.0f;
		vp.m[3][0] = 1920.0f / 2.0f, vp.m[3][1] = 1080.0f / 2.0f;
		invViewport = Matrix4Inverse(vp);

		Matrix4 tmp = invViewport * invPrj * invView;
		start = Matrix4Transform(mousePos, tmp);
		mousePos.z = 1.0f;
		end = Matrix4Transform(mousePos, tmp);

		Vector3 dir = end - start;
		dir.normalize();
		mouseRay_->SetDir(dir);
		mouseRay_->SetAttribute(COL_DEBUG);
		mouseRay_->SetOffSet(debugCamera_->GetEye());

		debugGoal_ = debugCamera_->GetEye() + (dir * 15.0f) + Vector3(0.0f, 1.0f, 0.0f);
		debugGoal_ = debugGoal_;
	}

	else
	{
		mouseRay_->SetAttribute(0);
		//mouseRay_->SetDir(Vector3());
	}

#pragma endregion

	ImGui::Begin("Enemy");
	ImGui::SliderFloat("CreateEnemyPosX", &enemyCreatePos.x, -100.0f, 100.0f);
	ImGui::SliderFloat("CreateEnemyPosY", &enemyCreatePos.y, -100.0f, 100.0f);
	ImGui::SliderFloat("CreateEnemyPosZ", &enemyCreatePos.z, -100.0f, 100.0f);
	//ImGui::Text("inter = { %f, %f, %f }")
	if (ImGui::Button("CreateEnemy"))
	{
		//CreateAddEnemy0(enemyCreatePos, enemyCreatePos);
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

void EnemyManager::ZombieUpdate()
{
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

	for (auto it = zombies0_.begin(); it != zombies0_.end();)
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
			it = zombies0_.erase(it);
			enemyDeathCounter_++;
			killEnemy_++;
		}
		else ++it;
	}

	for (auto it = zombies1_.begin(); it != zombies1_.end();)
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
			it = zombies1_.erase(it);
			enemyDeathCounter_++;
			killEnemy_++;
		}
		else ++it;
	}

	for (auto it = zombies2_.begin(); it != zombies2_.end();)
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
			it = zombies2_.erase(it);
			enemyDeathCounter_++;
			killEnemy_++;
		}
		else ++it;
	}

	for (auto it = zombies3_.begin(); it != zombies3_.end();)
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
			it = zombies3_.erase(it);
			enemyDeathCounter_++;
			killEnemy_++;
		}
		else ++it;
	}
}

void EnemyManager::BoidSeparation()
{
	for (size_t i = 0; i < zombies0_.size(); i++)
	{
		Vector3 vec = Vector3();
		float min = FLT_MAX;
		size_t index = 0;
		for (size_t j = 0; j < zombies0_.size(); j++)
		{
			if (i == j) continue;

			Vector3 zombie2zombie = zombies0_[i]->GetPosition() - zombies0_[j]->GetPosition();
			if (zombie2zombie.length() < min)
			{
				vec = zombie2zombie;
				min = zombie2zombie.length();
				index = j;
			}
		}

		zombies0_[i]->AddMoveVec(vec);
	}

	for (size_t i = 0; i < zombies1_.size(); i++)
	{
		Vector3 vec = Vector3();
		float min = FLT_MAX;
		size_t index = 0;
		for (size_t j = 0; j < zombies1_.size(); j++)
		{
			if (i == j) continue;

			Vector3 zombie2zombie = zombies1_[i]->GetPosition() - zombies1_[j]->GetPosition();
			if (zombie2zombie.length() < min)
			{
				vec = zombie2zombie;
				min = zombie2zombie.length();
				index = j;
			}
		}

		zombies1_[i]->AddMoveVec(vec);
	}
	
	for (size_t i = 0; i < zombies2_.size(); i++)
	{
		Vector3 vec = Vector3();
		float min = FLT_MAX;
		size_t index = 0;
		for (size_t j = 0; j < zombies2_.size(); j++)
		{
			if (i == j) continue;

			Vector3 zombie2zombie = zombies2_[i]->GetPosition() - zombies2_[j]->GetPosition();
			if (zombie2zombie.length() < min)
			{
				vec = zombie2zombie;
				min = zombie2zombie.length();
				index = j;
			}
		}

		zombies2_[i]->AddMoveVec(vec);
	}

	for (size_t i = 0; i < zombies3_.size(); i++)
	{
		Vector3 vec = Vector3();
		float min = FLT_MAX;
		size_t index = 0;
		for (size_t j = 0; j < zombies3_.size(); j++)
		{
			if (i == j) continue;

			Vector3 zombie2zombie = zombies3_[i]->GetPosition() - zombies3_[j]->GetPosition();
			if (zombie2zombie.length() < min)
			{
				vec = zombie2zombie;
				min = zombie2zombie.length();
				index = j;
			}
		}

		zombies3_[i]->AddMoveVec(vec);
	}
}

void EnemyManager::BoidCohesion()
{
	Vector3 center = Vector3();
	for (auto& it : zombies0_)
	{
		center += it->GetPosition();
	}
	center /= (float)zombies0_.size();

	for (auto& it : zombies0_)
	{
		it->AddMoveVec(center - it->GetPosition());
	}

	center = Vector3();
	for (auto& it : zombies1_)
	{
		center += it->GetPosition();
	}
	center /= (float)zombies1_.size();

	for (auto& it : zombies1_)
	{
		it->AddMoveVec(center - it->GetPosition());
	}

	center = Vector3();
	for (auto& it : zombies2_)
	{
		center += it->GetPosition();
	}
	center /= (float)zombies2_.size();

	for (auto& it : zombies2_)
	{
		it->AddMoveVec(center - it->GetPosition());
	}

	center = Vector3();
	for (auto& it : zombies3_)
	{
		center += it->GetPosition();
	}
	center /= (float)zombies3_.size();

	for (auto& it : zombies3_)
	{
		it->AddMoveVec(center - it->GetPosition());
	}
}

void EnemyManager::ZombieAllRushMode()
{
	for (auto& it : zombies_)
	{
		it->SetRushMode();
	}

	for (auto& it : zombies0_) it->SetRushMode();
	for (auto& it : zombies1_) it->SetRushMode();
	for (auto& it : zombies2_) it->SetRushMode();
	for (auto& it : zombies3_) it->SetRushMode();
}

void EnemyManager::ZombieRangeSearch(const Vector3& inPos)
{
	for (auto& it : zombies_)
	{
		Vector3 v = inPos - it->GetPosition();
		if (v.length() <= 10.0f) it->SetRushMode();
	}

	for (auto& it : zombies0_)
	{
		Vector3 v = inPos - it->GetPosition();
		if (v.length() <= 10.0f) it->SetRushMode();
	}

	for (auto& it : zombies1_)
	{
		Vector3 v = inPos - it->GetPosition();
		if (v.length() <= 10.0f) it->SetRushMode();
	}

	for (auto& it : zombies2_)
	{
		Vector3 v = inPos - it->GetPosition();
		if (v.length() <= 10.0f) it->SetRushMode();
	}

	for (auto& it : zombies3_)
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
	pPlayer_ = player;
}