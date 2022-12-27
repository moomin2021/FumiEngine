#pragma once
#include "BaseScene.h"
#include "Sprite.h"// -> スプライト
#include "Model.h"// -> モデルクラス
#include "Camera.h"
#include "Object3D.h"
#include "Key.h"

class GameScene : public BaseScene {
	// --メンバ変数-- //
public:

private:
	// キーボード入力
	Key* key_;

	// モデル
	Model* blackFloor_;// -> 黒色の床
	
	// オブジェクト
	Object3D* floor_[100];// -> 床

	// 各オブジェクトの数
	size_t maxFloor_;

	// カメラ
	Camera* camera_;

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