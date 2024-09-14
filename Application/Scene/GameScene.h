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
	void Initialize() override;
	void Update() override;
	void MatUpdate() override;
	void ImGuiUpdate() override;
	void Draw() override;
	void Finalize() override;
#pragma endregion
};
