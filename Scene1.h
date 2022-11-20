#pragma once
// --各シーンの継承クラス-- //
#include "BaseScene.h"

// --カメラクラス-- //
#include "Camera.h"

// --オブジェクト3Dクラス-- //
#include "Object3D.h"

// --キーボード入力クラス-- //
#include "Key.h"

class Scene1 : public BaseScene {
	// --メンバ関数-- //
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

	// --メンバ変数-- //
public:

private:
	// キーボード
	Key* key_;

	// カメラ
	Camera* camera_;

	// オブジェクト
	Object3D* object_;

	// 画像ハンドル
	int valoHandle_;
};