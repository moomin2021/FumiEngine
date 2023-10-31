#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Key.h"

#include "Stage.h"
#include "Player.h"
#include "EnemyManager.h"
#include "NavMesh.h"

#include <memory>

class MapCreateScene : public BaseScene
{
#pragma region メンバ変数
private:
	
#pragma endregion

#pragma region メンバ関数
public:
	MapCreateScene() {}
	~MapCreateScene();

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

	// デバック
	void Debug();
#pragma endregion
};
