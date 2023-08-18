#pragma once
#include "BaseScene.h"
#include "LightGroup.h"
#include "StageManager.h"
#include "EnemyManager.h"
#include "Player.h"

#include <memory>

class GameScene : public BaseScene
{
#pragma region 構造体
	// レベルデータ
	struct LevelData {
		// オブジェクト1個分のデータ
		struct ObjectData {
			// ファイル名
			int isActive;
			std::string fileName;
			std::string className;
			float3 translation;
			float3 rotation;
			float3 scaling;
		};

		std::vector<ObjectData> objects;
	};
#pragma endregion

#pragma region メンバ変数
private:
	// ライト
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// ステージマネージャー
	std::unique_ptr<StageManager> stageMgr_ = nullptr;

	// エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyMgr_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
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
	// オブジェクト更新処理
	void ObjUpdate();

	// 衝突時処理
	void OnCollision();

	// ステージを読み込む
	void LoadStage(std::string fileName);
#pragma endregion
};