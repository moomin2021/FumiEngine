#include "StageObjectManager.h"
#include "PipelineManager.h"

StageObjectManager::StageObjectManager() {}

StageObjectManager::~StageObjectManager()
{
}

void StageObjectManager::Initialize()
{
}

void StageObjectManager::Update()
{
}

void StageObjectManager::Draw()
{
	// 描画設定をオブジェクト3D用に
	PipelineManager::PreDraw("Object3D");

	// オブジェクト描画処理
	for (auto& object : objects_) object->Draw();
}

void StageObjectManager::OnCollision()
{
}

void StageObjectManager::MatUpdate()
{
	// オブジェクト行列更新
	for (auto& object : objects_) object->MatUpdate();
}