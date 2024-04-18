#pragma once
#include "CollisionManager2D.h"
#include "Sprite.h"
#include "BoxCollider.h"
#include "Mouse.h"

#include <memory>

class Slider
{
#pragma region メンバ変数
private:
	// インスタンス
	static CollisionManager2D* stColMgr2D_;
	Mouse* mouse_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sTrack_ = nullptr;
	std::unique_ptr<Sprite> sThumb_ = nullptr;

	// 画像
	uint16_t gTrack_ = 0;
	uint16_t gThumb_ = 0;

	// コライダー
	std::unique_ptr<BoxCollider> cThumb_ = nullptr;

	// Thumbの最小の位置と最大の位置
	Vector2 minPos_ = { 0.0f, 0.0f };
	Vector2 maxPos_ = { 0.0f, 0.0f };

	float value_ = 1.0f;

	// スライダーを操作中か
	bool isHold_ = false;
#pragma endregion

#pragma region メンバ関数
public:
	Slider() {}
	~Slider();

	// 初期化処理
	void Initialize(const Vector2& inTrackPos, const Vector2& inTrackSize, const Vector2& inThumbSize,
		uint16_t inTrackHandle, uint16_t inThumbHandle);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion

#pragma region セッター関数
public:
	static void SetCollisionManager2D(CollisionManager2D* inst) { stColMgr2D_ = inst; }

	// 衝突判定の[ON][OFF]を切り替える
	void SetIsCollision(bool frag);
#pragma endregion

#pragma region ゲッター関数
	float GetValue() { return value_; }
#pragma endregion
};