#include "ParticleManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager inst;
	return &inst;
}

void ParticleManager::Update()
{
}

void ParticleManager::Draw()
{
}

ParticleManager::ParticleManager() {}