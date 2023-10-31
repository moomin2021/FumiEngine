#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Model.h"
#include "Object3D.h"
#include "MapCreatePlayer.h"

#include <memory>

class MapCreateScene : public BaseScene
{
#pragma region メンバ変数
private:
	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// ライトグループ
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// スカイドーム
	std::unique_ptr<Model> mSkydome_ = nullptr;// モデル
	std::unique_ptr<Object3D> oSkydome_ = nullptr;// オブジェクト

	// プレイヤー
	std::unique_ptr<MapCreatePlayer> player_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	MapCreateScene() {}
	~MapCreateScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	// デバック
	void Debug();
#pragma endregion
};
