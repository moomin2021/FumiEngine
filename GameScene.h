#pragma once
#include "BaseScene.h"// -> ベースシーンクラス
#include "Sprite.h"// -> スプライト
#include "Model.h"// -> モデルクラス
#include "Camera.h"// -> カメラクラス
#include "Object3D.h"// -> オブジェクト3D
#include "Key.h"// -> キーボード入力
#include "Player.h"// -> プレイヤークラス
#include "Stage.h"

class GameScene : public BaseScene {
	// --メンバ変数-- //
public:

private:
	// キーボード入力
	Key* key_;

	// カメラ
	Camera* camera_;

	// プレイヤー
	Player* player_;

	// ステージ
	Stage* stage_;

	// モデル
	Model* blackFloorM_;// -> 黒色の床
	Model* whiteFloorM_;// -> 白色の床
	Model* wallM_;// -> 壁
	
	// 各オブジェクトの数
	static const size_t maxFloor_ = 169;

	// オブジェクト
	Object3D* floor_[maxFloor_];// -> 床

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
	// 当たり判定処理
	void Collision();

};