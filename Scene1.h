#pragma once
#include "LoadBlender.h"
#include "BaseScene.h"
#include "Sprite.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
#include <vector>
#include <memory>

class Scene1 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_;

	// カメラ
	std::unique_ptr<Camera> camera_;

	std::unique_ptr<LoadBlender> loadBle_;

	// ライト
	std::unique_ptr<LightGroup> lightGroup_;
	std::unique_ptr<DirectionalLight> dirLight_;
	std::unique_ptr<PointLight> pointLight_;

	// モデル
	std::unique_ptr<Model> mFloor_;
	std::unique_ptr<Model> mCube_;

	// オブジェクト
	std::unique_ptr<Object3D> oFloor_;
	std::vector<std::unique_ptr<Object3D>> oCube_;

	// テクスチャハンドル
	uint16_t haeHandle_;

	// スプライト
	std::unique_ptr<Sprite> sHae_;

	// --メンバ関数-- //
public:
	// コンストラクタ
	Scene1();

	// デストラクタ
	~Scene1();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
};

