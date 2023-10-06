#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "LightGroup.h"
#include "DirectionalLight.h"

#include "Stage.h"
#include "Player.h"
#include "EnemyManager.h"

#include <memory>

class GameScene : public BaseScene
{
#pragma region メンバ変数
private:
	// ライトグループ
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// ステージオブジェクトの管理クラス
	std::unique_ptr<Stage> stage_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyMgr_ = nullptr;

	// モデル
	std::unique_ptr<Model> mCube_ = nullptr;

	// 3軸を示すオブジェクト
	std::vector<std::unique_ptr<Object3D>> oAxis_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	GameScene();

	// デストラクタ
	~GameScene();

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
#pragma endregion
};