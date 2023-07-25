#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Key.h"
#include "Dissolve3D.h"

#include <memory>

class Scene3 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// ライト
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_;

	// オブジェクト
	std::unique_ptr<Object3D> object_;

	// --メンバ関数-- //
public:
	// コンストラクタ
	Scene3();

	// デストラクタ
	~Scene3();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
};

