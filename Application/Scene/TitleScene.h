#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Mouse.h"
#include "Sprite.h"
#include "Camera.h"

#include "CollisionManager2D.h"
#include "PointCollider.h"
#include "BoxCollider.h"

#include "Stage.h"
#include "TitleLayer.h"
#include "SettingLayer.h"
#include "GamePlayLayer.h"
#include "AudioLayer.h"

#include <memory>

class TitleScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	CollisionManager2D* colMgr2D_ = nullptr;
	Mouse* mouse_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// ライトグループ
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// ステージ
	std::unique_ptr<Stage> stage_ = nullptr;

	// コライダー
	std::unique_ptr<PointCollider> cMouse_ = nullptr;

	// タイトルレイヤー
	std::unique_ptr<TitleLayer> titleLayer_ = nullptr;

	// 設定レイヤー
	std::unique_ptr<SettingLayer> settingLayer_ = nullptr;

	// ゲームプレイレイヤー
	std::unique_ptr<GamePlayLayer> gamePlayLayer_ = nullptr;

	// オーディオレイヤー
	std::unique_ptr<AudioLayer> audioLayer_ = nullptr;
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
	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	// カメラを回転
	void CameraRota();
#pragma endregion
};
