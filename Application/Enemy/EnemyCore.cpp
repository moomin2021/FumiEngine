#include "EnemyCore.h"
#include "CollisionAttribute.h"

LightGroup* EnemyCore::sLightGroup_ = nullptr;
CollisionManager* EnemyCore::sColMgr_ = nullptr;
Model* EnemyCore::sCoreM_ = nullptr;
Model* EnemyCore::sCoreFrameM_ = nullptr;
Model* EnemyCore::sCoreStandM_ = nullptr;

void EnemyCore::Initialize(const Vector3& inPos)
{
#pragma region オブジェクト
	// 生成
	coreO_ = std::make_unique<Object3D>(sCoreM_);
	coreFrameO_ = std::make_unique<Object3D>(sCoreFrameM_);
	coreStandO_ = std::make_unique<Object3D>(sCoreStandM_);

	// 位置設定
	coreO_->SetPosition(inPos + offset1_);
	coreFrameO_->SetPosition(inPos + offset1_);
	coreStandO_->SetPosition(inPos + offset0_);
#pragma endregion

#pragma region コライダー
	collider_ = std::make_unique<SphereCollider>();
	collider_->SetObject3D(coreO_.get());
	collider_->SetRadius(0.8f);
	collider_->SetAttribute(COL_ENEMY_CORE);
	sColMgr_->AddCollider(collider_.get());
#pragma endregion

#pragma region ライト
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->SetLightPos(inPos + offset1_);
	pointLight_->SetLightColor({0.5f, 1.0f, 1.0f});
	pointLight_->SetLightAtten({1.0f, 1.0f, 1.0f});
	sLightGroup_->AddPointLight(pointLight_.get());
#pragma endregion
}

void EnemyCore::Update()
{
	lightAtt_ -= subLightAtt_;
	lightAtt_ = Util::Clamp(lightAtt_, maxLightAtt_, minLightAtt_);
	pointLight_->SetLightAtten({ lightAtt_, lightAtt_ , lightAtt_ });
}

void EnemyCore::Draw()
{
	coreO_->Draw();
	coreFrameO_->Draw();
	coreStandO_->Draw();
}

void EnemyCore::OnCollision()
{
	if (collider_->GetIsHit())
	{
		lightAtt_ = maxLightAtt_;
		hp_ -= 1;

		if (hp_ <= 0) isAlive_ = false;
	}
}

void EnemyCore::MatUpdate()
{
	coreO_->MatUpdate();
	coreFrameO_->MatUpdate();
	coreStandO_->MatUpdate();
}

EnemyCore::~EnemyCore()
{
	sColMgr_->RemoveCollider(collider_.get());
}