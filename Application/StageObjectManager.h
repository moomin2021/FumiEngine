#pragma once
#include "Model.h"
#include "Object3D.h"
#include "MeshCollider.h"
#include "CollisionManager.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <forward_list>

class StageObjectManager
{
#pragma region メンバ変数
private:
	// 衝突判定管理クラスインスタンス
	CollisionManager* colMgr_ = nullptr;

	// モデル保存用連想配列
	std::unordered_map<std::string, std::unique_ptr<Model>> models_ = {};

	// オブジェクト
	std::forward_list<std::unique_ptr<Object3D>> objects_ = {};

	// コライダー
	std::forward_list<std::unique_ptr<MeshCollider>> colliders_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	StageObjectManager();

	// デストラクタ
	~StageObjectManager();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時更新処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion
};