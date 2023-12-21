#include "DeltaTime.h"
#include "Util.h"

void DeltaTime::Initialize()
{
	oldTimeMS_ = Util::GetTimrMSec();
}

void DeltaTime::Update()
{
	deltaTime_ = (Util::GetTimrMSec() - oldTimeMS_) / 1000.0f;
	oldTimeMS_ = Util::GetTimrMSec();
}