#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Key.h"
#include "AABBCollider.h"
#include "CollisionManager.h"

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

	// ライト
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// モデル
	std::unique_ptr<Model> cubeM_ = nullptr;

	// オブジェクト
	const uint8_t cubeMax_ = 3;
	std::unique_ptr<Object3D[]> cubeO_ = nullptr;
	Vector3 cube1Pos_ = { 0.0f, 0.0f, -1.5f };

	// コライダー
	std::unique_ptr<AABBCollider[]> cubeC_ = nullptr;
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
