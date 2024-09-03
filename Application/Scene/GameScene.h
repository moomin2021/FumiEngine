#pragma once
#include "BaseScene.h"
#include "CameraManager.h"
#include "DebugCamera.h"

#include <memory>

class GameScene : public BaseScene
{
#pragma region メンバ変数
private:
	// カメラマネージャー
	std::unique_ptr<CameraManager> cameraMgr_ = nullptr;

	// デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタとデストラクタ
	GameScene(IScene* sceneIf);
	~GameScene() {}

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
