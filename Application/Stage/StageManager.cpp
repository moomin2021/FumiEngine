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

	// モデル
	mSkydome_ = std::make_unique<Model>("skydome");

	// オブジェクト
	oSkydome_ = std::make_unique<Object3D>(mSkydome_.get());
	oSkydome_->SetScale({ 1000.0f, 1000.0f, 1000.0f });
}

void StageManager::ObjUpdate()
{
	// オブジェクト更新処理
	for (auto& it : stageObjects_) {
		it->Update();
	}

	oSkydome_->Update();
}

void StageManager::OnCollision()
{
	//for (size_t i = 0; i < stageObjColliders_.size(); i++) {
	//	if (stageObjColliders_[i]->GetIsHit()) {
	//		stageObjects_[i]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	//	}

	//	else {
	//		stageObjects_[i]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	//	}
	//}
}

void StageManager::Draw()
{
	// オブジェクト描画
	for (auto& it : stageObjects_) {
		it->Draw();
	}

	oSkydome_->Draw();
}

void StageManager::AddModel(std::string fileName)
{
	// 同じ名前のモデルが無かったらモデルを追加
	if (stageModels_.count(fileName) == 0) {
		stageModels_.emplace(fileName, std::make_unique<Model>(fileName));
	}
}

void StageManager::AddObject3D(std::string modelName, const Vector3& position, const Vector3& rotation, const Vector3& scale)
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
	stageObjColliders_.back()->SetObject3D(stageObjects_.back().get());

	// コライダーを登録
	colMgr_->AddCollider(stageObjColliders_.back().get());
}