#include "EnemyCore.h"
#include "CollisionAttribute.h"

void EnemyCore::Initialize(const Vector3& inPos)
{
	// オブジェクト生成＆設定
	coreO_ = std::make_unique<Object3D>(pCoreM_);
	coreFrameO_ = std::make_unique<Object3D>(pCoreFrameM_);
	coreStandO_ = std::make_unique<Object3D>(pCoreStandM_);

	coreO_->SetPosition(inPos + offset1_);
	coreFrameO_->SetPosition(inPos + offset1_);
	coreStandO_->SetPosition(inPos + offset0_);

	// コライダーの生成＆設定＆登録
	coreC_ = std::make_unique<AABBCollider>();
	coreC_->SetAttribute(COL_ENEMY_CORE);
	coreC_->SetRadius({ 0.8f, 0.8f, 0.8f });
	coreC_->SetObject3D(coreO_.get());
	pColMgr_->AddCollider(coreC_.get());

	// ライトの生成＆設定＆登録
	pointLight_ = std::make_unique<PointLight>();
	lightPosition_ = inPos + offset1_;
	pointLight_->SetPosition(lightPosition_);
	pointLight_->SetColor({ 0.5f, 1.0f, 1.0f });
	pointLight_->SetRadius(lightRadius_);
	pLightGroup_->AddPointLight(pointLight_.get());
}

void EnemyCore::Update()
{
	// ライトの更新
	lightIntensity_ -= subLightIntensity_;
	lightIntensity_ = Util::Clamp(lightIntensity_, maxLightIntensity_, minLightIntensity_);
	pointLight_->SetPosition(lightPosition_);
	pointLight_->SetRadius(lightRadius_);
	pointLight_->SetIntensity(lightIntensity_);
}

void EnemyCore::Draw()
{
	coreO_->Draw();
	coreFrameO_->Draw();
	coreStandO_->Draw();
}

void EnemyCore::Collision()
{
	Hit();
}

void EnemyCore::MatUpdate()
{
	coreO_->MatUpdate();
	coreFrameO_->MatUpdate();
	coreStandO_->MatUpdate();
}

void EnemyCore::Finalize()
{
	pColMgr_->RemoveCollider(coreC_.get());
	pLightGroup_->RemovePointLight(pointLight_.get());
}

void EnemyCore::Hit()
{
	if (coreC_->GetIsHit() == false) return;
	if (coreC_->GetHitCollider()->GetAttribute() != COL_PLAYER_SHOT) return;

	lightIntensity_ = maxLightIntensity_;
	hp_ -= 1;

	if (hp_ <= 0) isAlive_ = false;
}

void EnemyCore::SetModel(Model* inCoreM, Model* inCoreFrameM, Model* inCoreStandM)
{
	pCoreM_ = inCoreM;
	pCoreFrameM_ = inCoreFrameM;
	pCoreStandM_ = inCoreStandM;
}