#pragma once
#include "Object3D.h"
#include "MeshCollider.h"
#include "CollisionManager.h"

#include <memory>
#include <deque>

class StageManager
{
#pragma region メンバ変数
private:
	// インスタンス
	CollisionManager* colMgr_ = nullptr;

	// ステージの各モデル
	std::map<std::string, std::unique_ptr<Model>> stageModels_;

	// ステージの各オブジェクト
	std::deque<std::unique_ptr<Object3D>> stageObjects_;

	// ステージの各オブジェクトのコライダー
	std::deque<std::unique_ptr<MeshCollider>> stageObjColliders_;
#pragma endregion

#pragma region メンバ関数
public:
	// デストラクタ
	~StageManager();

	// 初期化処理
	void Initialize();

	// 更新処理
	void ObjUpdate();

	// 衝突時処理
	void OnCollision();

	// 描画処理
	void Draw();

	// モデルを追加
	void AddModel(std::string fileName);

	// オブジェクト3Dの追加
	void AddObject3D(std::string modelName, const float3& position, const float3& rotation, const float3& scale);
#pragma endregion
};