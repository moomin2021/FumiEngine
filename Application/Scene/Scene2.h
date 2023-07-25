#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Key.h"
#include "Dissolve3D.h"

#include <memory>

class Scene2 : public BaseScene
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

	// ディゾルブ
	std::unique_ptr<Dissolve3D> dissolve_ = nullptr;

	// --メンバ関数-- //
public:
	// コンストラクタ
	Scene2();

	// デストラクタ
	~Scene2();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
};

