#pragma once
#include "Sprite.h"
#include "CollisionManager2D.h"
#include "BoxCollider.h"
#include "PointCollider.h"
#include "Mouse.h"

#include "BoxButton.h"

#include <vector>
#include <memory>

enum class SelectState {
	NONE = -1,
	GAME_PLAY,
	AUDIO,
};

class SettingLayer
{
#pragma region メンバ変数
private:
	// インスタンス
	Mouse* mouse_ = nullptr;
	CollisionManager2D* colMgr2D_ = nullptr;

	// スプライト	
	std::unique_ptr<Sprite> sTreeButtonCursorFrame_ = nullptr;
	std::unique_ptr<Sprite> sBackBox_ = nullptr;

	// 画像
	uint16_t gTreeButtonCursorFrame_ = 0;
	uint16_t gBackBox_ = 0;

	// コライダー
	std::unique_ptr<PointCollider> mouseCol_ = nullptr;

	// ツリーボタン関連
	bool isTreeButton_ = false;// ボタンを選択しているか
	SelectState cursorState_ = SelectState::NONE;
	SelectState treeState_ = SelectState::GAME_PLAY;
	std::vector<Vector2> treeButtonPos_ = {};// ボタンの座標
	Vector2 startTreeButtonFrameSize_ = { 285.0f, 65.0f };
	Vector2 endTreeButtonFrameSize_ = { 272.0f, 56.0f };
	float easeTime_ = 0.5f;// [s]
	uint64_t startEaseTime_ = 0;

	// ボタン
	std::vector<std::unique_ptr<BoxButton>> treeButtons_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	SettingLayer() {}
	~SettingLayer();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

private:
	// ボタンフレームの処理
	void ButtonFrame();
#pragma endregion
};