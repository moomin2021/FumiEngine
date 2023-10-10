#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Mouse.h"
#include "Sprite.h"
#include "Camera.h"
#include "SettingLayer.h"

#include "CollisionManager2D.h"
#include "PointCollider.h"
#include "BoxCollider.h"

#include "Stage.h"

#include <memory>

class TitleScene : public BaseScene
{
	enum class SelectNum {
		NONE = -1,
		START,
		SETTING,
		END,
	};

#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;
	CollisionManager2D* colMgr2D_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// ライトグループ
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sTitle_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sSelectButtons_ = {};
	std::unique_ptr<Sprite> sSelectButtonFrame_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sSelectText_ = {};

	// 画像ハンドル
	uint16_t gTitle_ = 0;
	uint16_t gSelectButton_ = 0;
	uint16_t gSelectButtonFrame_ = 0;
	std::vector<uint16_t> gSelectText_ = {};

	// コライダー
	std::unique_ptr<PointCollider> mouseCol_ = nullptr;
	std::vector<std::unique_ptr<BoxCollider>> selectButtonsCol_ = {};

	// セレクトボタン関連
	bool isSelect_ = false;// ボタンを選択しているか
	SelectNum nowSelect_ = SelectNum::NONE;
	std::vector<Vector2> selectButtonPos_ = {};// ボタンの座標
	Vector2 startSelectButtonFrameSize_ = { 330.0f, 60.0f };
	Vector2 endSelectButtonFrameSize_ = { 324.0f, 54.0f };
	float easeTime_ = 0.5f;// [s]
	uint64_t startEaseTime_ = 0;

	// ステージ
	std::unique_ptr<Stage> stage_ = nullptr;

	// 設定レイヤー
	std::unique_ptr<SettingLayer> settingLayer_ = nullptr;
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

	// セレクトボタンの処理
	void SelectButton();

	// カメラを回転
	void CameraRota();
#pragma endregion
};
