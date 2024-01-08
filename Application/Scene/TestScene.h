#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Key.h"

#include "Stage.h"
#include "Player.h"
#include "PlayerUI.h"
#include "EnemyManager.h"
#include "NavMesh.h"
#include "DeltaTime.h"
#include "CollisionManager.h"
#include "AABBCollider.h"

#include <memory>

class TestScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	LightGroup* lightGroup_ = nullptr;
	CollisionManager* colMgr_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_ = nullptr;

	// オブジェクト
	std::vector<std::unique_ptr<Object3D>> objects_ = {};

	// コライダー
	std::vector<std::unique_ptr<AABBCollider>> colliders_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	TestScene();

	// デストラクタ
	~TestScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// デバック
	void Debug();

private:
	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion
};
