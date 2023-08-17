#include "StageManager.h"
#include "CollisionAttribute.h"

StageManager::~StageManager()
{
	for (auto& it : stageObjColliders_) {
		colMgr_->RemoveCollider(it.get());
	}
}

void StageManager::Initialize()
{
	// コリジョンマネージャーインスタンス取得
	colMgr_ = CollisionManager::GetInstance();
}

void StageManager::ObjUpdate()
{
	// オブジェクト更新処理
	for (auto& it : stageObjects_) {
		it->Update();
	}
}

void StageManager::Draw()
{
	// オブジェクト描画
	for (auto& it : stageObjects_) {
		it->Draw();
	}
}

void StageManager::AddModel(std::string fileName)
{
	// 同じ名前のモデルが無かったらモデルを追加
	if (stageModels_.count(fileName) == 0) {
		stageModels_.emplace(fileName, std::make_unique<Model>(fileName));
	}
}

void StageManager::AddObject3D(std::string modelName, const float3& position, const float3& rotation, const float3& scale)
{
	// モデル名を指定して生成
	stageObjects_.emplace_back(std::make_unique<Object3D>(stageModels_[modelName].get()));

	// データを設定
	stageObjects_.back()->SetPosition(position);
	stageObjects_.back()->SetRotation(rotation);
	stageObjects_.back()->SetScale(scale);

	// コライダーを生成
	stageObjColliders_.emplace_back(std::make_unique<MeshCollider>(stageObjects_.back().get()));
	stageObjColliders_.back()->SetAttribute(COL_STAGE_OBJ);
	stageObjColliders_.back()->LinkObject3D(stageObjects_.back().get());

	// コライダーを登録
	colMgr_->AddCollider(stageObjColliders_.back().get());
}