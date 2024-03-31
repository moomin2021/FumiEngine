#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Model.h"
#include "Object3D.h"

#include <memory>

class MapCreateScene : public BaseScene
{
#pragma region メンバ変数
private:
	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// ライトグループ
	LightGroup* lightGroup_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// スカイドーム
	std::unique_ptr<Model> mSkydome_ = nullptr;// モデル
	std::unique_ptr<Object3D> oSkydome_ = nullptr;// オブジェクト

	// モデル
	std::unique_ptr<Model> model_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	MapCreateScene(IScene* sceneIf);
	~MapCreateScene() {}

	// 基本処理
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// 衝突時処理
	void Collision();

	// 行列更新処理
	void MatUpdate();

	// デバック
	void Debug();
#pragma endregion
};
