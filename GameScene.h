#pragma once
#include "BaseScene.h"// -> シーンのベース
#include "Sprite.h"// -> スプライト
#include "Model.h"// -> モデルクラス
#include "Camera.h"
#include "Object3D.h"

class GameScene : public BaseScene {
	// --メンバ変数-- //
public:

private:
	// スプライト
	Sprite* sprite_;

	Model* model_;

	Camera* camera_;

	int textureHandle_;

	Object3D* object_;

	// --メンバ関数-- //
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

};