#pragma once
<<<<<<< HEAD:GameScene.h
#include "BaseScene.h"// -> ベースシーンクラス
#include "Sprite.h"// -> スプライト
#include "Model.h"// -> モデルクラス
#include "Camera.h"// -> カメラクラス
#include "Object3D.h"// -> オブジェクト3D
#include "Key.h"// -> キーボード入力
#include "Player.h"// -> プレイヤークラス
#include "Stage.h"
#include "Sprite.h"
=======
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
>>>>>>> master:Scene1.h

class Scene1 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_;

	// カメラ
	Camera* camera_;

<<<<<<< HEAD:GameScene.h
	// プレイヤー
	Player* player_;

	// ステージ
	Stage* stage_;
	
	// 各オブジェクトの数
	static const size_t maxFloor_ = 169;

	int openOrCloseH_;
	Sprite* openOrClose_;
	bool isText_;

<<<<<<< HEAD:Scene1.h
	std::unique_ptr<Sprite> hintText_;
	int hintTextH_;
=======
	// ライト
	LightGroup* lightGroup_;

	// モデル
	Model* sphereM_;
	Model* cubeM_;
	Model* floorM_;

	// オブジェクト
	Object3D* object_[3];
>>>>>>> master:Scene1.h

=======
>>>>>>> parent of fcb6802 (邨ゆｺ�):GameScene.h
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
};

<<<<<<< HEAD:GameScene.h
private:
	// 当たり判定処理
	void Collision();

	void WallCol();// -> 壁とプレイヤーの当たり判定
	void WallSlide(Line2D& wall);// -> 壁ずり処理

	void DoorCol();

};
=======
>>>>>>> master:Scene1.h
