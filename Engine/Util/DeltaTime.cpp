#include "DeltaTime.h"
#include "Util.h"

void DeltaTime::Initialize()
{
	oldTimeMS_ = Util::GetTimrMSec();
}

void DeltaTime::Update()
{
	deltaTime_ = ((Util::GetTimrMSec() - oldTimeMS_) / 1000.0f) * timeSpd_;
	gameTime_ += deltaTime_;
	oldTimeMS_ = Util::GetTimrMSec();
}

DeltaTime* DeltaTime::GetInstance()
{
	// インスタンス生成
	static DeltaTime inst;

	// インスタンスを返す
	return &inst;
}
