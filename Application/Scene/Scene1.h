#pragma once
#include "BaseScene.h"
#include "LightGroup.h"
#include "LoadStage.h"
#include "Player.h"
#include "EnemyManager.h"

#include <vector>

class Scene1 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_;

	// 敵管理クラス
	EnemyManager* enemyManager_;

	// カメラ
	std::unique_ptr<Camera> camera_;

	// ライト
	std::unique_ptr<LightGroup> lightGroup_;
	std::unique_ptr<DirectionalLight> dirLight_;

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
};

