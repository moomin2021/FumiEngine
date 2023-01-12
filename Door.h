#pragma once
#include "Object3D.h"
#include "Model.h"
#include "ColType.h"
#include <memory>

class Door
{
	// メンバ変数
public:
	std::unique_ptr<Object3D> object_;
	//Object3D object_;// -> オブジェクト
	Vector3 oldPos_;

	Line2D col2D_;// -> 2D用当たり判定
	Board col3D_;// -> 3D用当たり判定

	bool isRight_;// -> ドアが右にあるか
	bool isMove_;

private:
	int timer_;
	int maxTime_;

	// メンバ関数
public:
	Door();// --> コンストラクタ
	~Door();// -> デストラクタ
	void Initialize();
	void Update();// -----> 更新処理
	void Draw();// -------> 描画処理

	void SetModel(Model* model) { object_->SetModel(model); }
	void SetCamera(Camera* camera) { object_->SetCamera(camera); }
};

