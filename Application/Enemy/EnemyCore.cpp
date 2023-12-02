#include "EnemyCore.h"

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
}

void EnemyCore::Update()
{
}

void EnemyCore::Draw()
{
	coreO_->Draw();
	coreFrameO_->Draw();
	coreStandO_->Draw();
}

void EnemyCore::OnCollision()
{
}

void EnemyCore::MatUpdate()
{
	coreO_->MatUpdate();
	coreFrameO_->MatUpdate();
	coreStandO_->MatUpdate();
}

EnemyCore::~EnemyCore()
{
}