#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Key.h"
#include "CollisionPrimitive.h"

class Scene4 : public BaseScene
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
	Model* triangleM_;

	// オブジェクト
	Object3D* object_[3];

	// 当たり判定 レイ
	Ray rayC_;

	// 当たり判定 三角形
	Triangle triangleC_;

	// カウント
	int count_;

	// --メンバ関数-- //
public:
	// コンストラクタ
	Scene4();

	// デストラクタ
	~Scene4();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
};

