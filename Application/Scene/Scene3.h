#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Mouse.h"
#include "PointCollider.h"
#include "BoxCollider.h"

#include <memory>

class Scene3 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;

	std::unique_ptr<PointCollider> pCol_ = nullptr;
	std::unique_ptr<BoxCollider> boxCol_ = nullptr;

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

