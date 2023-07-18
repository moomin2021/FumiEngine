#pragma once
#include "BaseScene.h"
#include "LightGroup.h"
#include "LoadStage.h"
#include "Player.h"

#include <vector>

class Scene1 : public BaseScene
{
	// --メンバ変数-- //
private:
	// ライト
	std::unique_ptr<LightGroup> lightGroup_;
	std::unique_ptr<DirectionalLight> dirLight_;

	// モデル
	std::unique_ptr<Model> mFloor_;
	std::unique_ptr<Model> mCube_;
	std::unique_ptr<Model> mSphere_;

	// オブジェクト
	std::unique_ptr<Object3D> oFloor_;
	std::unique_ptr<Object3D> oSphere_;
	std::vector<std::unique_ptr<Object3D>> oCube_;

	// テクスチャハンドル
	uint16_t haeHandle_;
	// ステージ読み込み用クラス
	std::unique_ptr<LoadStage> loadStage_;

	// プレイヤー
	std::unique_ptr<Player> player_;

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

	void Collision();
};

