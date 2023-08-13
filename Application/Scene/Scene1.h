#pragma once
#include "BaseScene.h"
#include "LightGroup.h"
#include "EnemyManager.h"
#include "Player.h"
#include "MeshCollider.h"
#include "ItemManager.h"

class Scene1 : public BaseScene {
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

	// エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// ステージオブジェクト
	std::map<std::string, std::unique_ptr<Model>> stageModels_;// モデル連想配列
	std::list<std::unique_ptr<Object3D>> stageObjects_;// オブジェクト配列
	std::list<std::unique_ptr<MeshCollider>> stageObjCollider_;
#pragma endregion

#pragma region メンバ関数
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

private:
	// ステージを読み込む
	void LoadStage(std::string fileName);
#pragma endregion
};