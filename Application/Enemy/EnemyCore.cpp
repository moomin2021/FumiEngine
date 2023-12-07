#include "EnemyCore.h"
#include "CollisionAttribute.h"

#include <imgui_impl_DX12.h>

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
	lightPosition_ = inPos + offset1_;
	pointLight_->SetPosition(lightPosition_);
	pointLight_->SetColor({ 0.5f, 1.0f, 1.0f });
	pointLight_->SetRadius(lightRadius_);
	sLightGroup_->AddPointLight(pointLight_.get());
#pragma endregion
}

void EnemyCore::Update()
{
	lightIntensity_ -= subLightIntensity_;
	lightIntensity_ = Util::Clamp(lightIntensity_, maxLightIntensity_, minLightIntensity_);
	pointLight_->SetPosition(lightPosition_);
	pointLight_->SetRadius(lightRadius_);
	pointLight_->SetIntensity(lightIntensity_);
	//pointLight_->SetDecay(dLightDecay_);
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
		lightIntensity_ = maxLightIntensity_;
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

void EnemyCore::Debug()
{
	ImGui::Begin("Core");
	ImGui::SliderFloat3("position", &lightPosition_.x, -5.0f, 5.0f);
	ImGui::SliderFloat("decay", &dLightDecay_, 0.0f, 10.0f);
	ImGui::End();
}

EnemyCore::~EnemyCore()
{
	sColMgr_->RemoveCollider(collider_.get());
	sLightGroup_->RemovePointLight(pointLight_.get());
}