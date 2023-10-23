#pragma once
#include "Sprite.h"
#include "CollisionManager2D.h"
#include "BoxCollider.h"
#include "PointCollider.h"
#include "Mouse.h"

#include "BoxButton.h"
#include "HitFrame.h"

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

	// ボタン
	std::vector<std::unique_ptr<BoxButton>> buttons_ = {};

	// フレーム関連
	uint16_t frameHandle_ = 0;
	const Vector2 frameSize_ = { 272.0f, 56.0f };
	std::unique_ptr<HitFrame> hitFrame_ = nullptr;
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
	void OnCollision(SelectNum& selectNum);

	// 行列更新処理
	void MatUpdate();

	// 衝突判定の[ON][OFF]を切り替える
	void SetIsCollision(bool frag);
#pragma endregion
};