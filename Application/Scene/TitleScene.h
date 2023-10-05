#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Mouse.h"
#include "Sprite.h"
#include "Camera.h"

#include "CollisionManager2D.h"
#include "PointCollider.h"
#include "BoxCollider.h"

#include <memory>

class TitleScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;
	CollisionManager2D* colMgr2D_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sTitle_ = nullptr;

	// テクスチャハンドル
	uint16_t hTitle_ = 0;

	// スプライト
	std::vector<std::unique_ptr<Sprite>> sSelectButtons_ = {};
	std::unique_ptr<Sprite> sSelectButtonFrame_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sSelectText_ = {};

	// 画像ハンドル
	uint16_t gSelectButton_ = 0;
	uint16_t gSelectButtonFrame_ = 0;
	std::vector<uint16_t> gSelectText_ = {};

	// コライダー
	std::unique_ptr<PointCollider> mouseCol_ = nullptr;
	std::vector<std::unique_ptr<BoxCollider>> selectButtonsCol_ = {};
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