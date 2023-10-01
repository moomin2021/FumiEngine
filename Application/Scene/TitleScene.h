#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Sprite.h"
#include "Camera.h"

#include "PointCollider.h"

#include <memory>

class TitleScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sTitle_ = nullptr;

	// テクスチャハンドル
	uint16_t hTitle_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	void OnCollision();

	void MatUpdate();
#pragma endregion
};