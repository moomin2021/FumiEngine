#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "Sprite.h"
#include "Mouse.h"

#include "CollisionManager2D.h"
#include "PointCollider.h"
#include "BoxButton.h"

#include "ResultBox.h"
#include "Number.h"

#include <memory>

class GameOverScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	CollisionManager2D* colMgr2D_ = nullptr;
	Mouse* mouse_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> resultFrameS_ = nullptr;
	std::unique_ptr<Sprite> resultS_ = nullptr;

	// 画像
	uint16_t resultFrameH_ = 0;
	std::vector<uint16_t> resultH_ = {};

	// ボタン
	std::unique_ptr<BoxButton> titleReturnB_ = nullptr;

	// コライダー
	std::unique_ptr<PointCollider> cMouse_ = nullptr;

	// リザルト表記
	std::vector<std::unique_ptr<ResultBox>> resultBoxs_ = {};

	// 数字表記
	std::vector<std::unique_ptr<Number>> numbers_ = {};
	std::vector<uint16_t> displayNums_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタとデストラクタ
	GameOverScene(IScene* sceneIf);
	~GameOverScene() {}

	// 基本処理
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// 衝突時処理
	void Collision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion
};
