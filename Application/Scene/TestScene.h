#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Key.h"

#include "Stage.h"
#include "Player.h"
#include "PlayerUI.h"
#include "NavMesh.h"
#include "CameraManager.h"
#include "DebugCamera.h"

#include <memory>

class TestScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	LightGroup* lightGroup_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// ステージオブジェクトの管理クラス
	std::unique_ptr<Stage> stage_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<PlayerUI> playerUI_ = nullptr;

	// カメラマネージャー
	std::unique_ptr<CameraManager> cameraMgr_ = nullptr;

	// デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	// デバック
	bool isDebug_ = false;

	// ゲームUI
	std::unique_ptr<Sprite> sGameUI_ = nullptr;
	uint16_t gGameUI_ = 0;

	std::unique_ptr<Sprite> sObjectiveText_ = nullptr;
	uint16_t gObjectiveText_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタとデストラクタ
	TestScene(IScene* sceneIf);
	~TestScene() {}

	// 基本処理
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	// デバック
	void Debug();

private:
	// 衝突時処理
	void Collision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion
};
