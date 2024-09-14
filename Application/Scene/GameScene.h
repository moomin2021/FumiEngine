#pragma once
#include "BaseScene.h"

#include <memory>

class GameScene : public BaseScene
{
#pragma region メンバ変数
private:
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
