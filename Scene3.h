#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
#include "CollisionPrimitive.h"

class Scene3 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_;

	// カメラ
	Camera* camera_;

	// ライト
	LightGroup* lightGroup_;

	// モデル
	Model* sphereM_;
	Model* rayM_;
	Model* planeM_;

	// オブジェクト
	Object3D* object_[3];

	// 当たり判定 レイ
	Ray rayC_;

	// 当たり判定 平面
	Plane planeC_;

	// カウント
	int count_;

	// --メンバ関数-- //
public:
	// コンストラクタ
	Scene3();

	// デストラクタ
	~Scene3();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
};

