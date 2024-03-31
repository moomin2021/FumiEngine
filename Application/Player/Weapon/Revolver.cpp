#include "Revolver.h"

Revolver::Revolver(Object3D* inObject) : Weapon(inObject) {}

void Revolver::Initialize()
{
	adsRateIncrDecValue_ = 0.2f;
	reloadTime_ = 3.0f;
	maxBullet_ = 6;
	nowBullet_ = 6;
	shotInterval_ = 0.15f;

	adsOffset_ = { 0.0f, 1.65f, 0.5f };
	noAdsOffset_ = { 0.5f, 1.55f, 0.5f };

	model_ = std::make_unique<Model>("Sheriff");
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetScale({ 0.1f, 0.1f, 0.1f });
}

void Revolver::Update()
{
	AdsProcess();
	ReloadProcess();
}

void Revolver::Draw()
{
	object_->Draw();
}

void Revolver::Collision()
{
}

void Revolver::MatUpdate()
{
	object_->MatUpdate();
}

void Revolver::Finalize()
{
}