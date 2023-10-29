#pragma once
#include "CollisionManager2D.h"
#include "BoxCollider.h"
#include "Sprite.h"

#include "ButtonAttribute.h"

#include <memory>

class BoxButton
{
#pragma region メンバ変数
private:
	// インスタンス
	static CollisionManager2D* stColMgr2D_;

	// スプライト
	std::unique_ptr<Sprite> sButton_ = nullptr;
	std::unique_ptr<Sprite> sText_ = nullptr;
	std::unique_ptr<Sprite> sFrame_ = nullptr;

	// 画像
	uint16_t gButton_ = 0;
	uint16_t gText_ = 0;
	uint16_t gFrame_ = 0;

	// コライダー
	std::unique_ptr<BoxCollider> cButton_ = nullptr;

	// イージング関連
	Vector2 size_ = { 0.0f, 0.0f };// 画像のサイズ
	float maxSizeRate_ = 1.1f;// 画像の最大拡大率
	float easeTime_ = 0.5f;// イージング時間
	uint64_t startEaseTime_ = 0;// イージング開始時間

	// 描画フラグ
	bool isDraw_ = false;
#pragma endregion

#pragma region メンバ関数
public:
	BoxButton() {}
	~BoxButton();

	// 初期化処理
	void Initialize(int32_t inTag, const Vector2& inPos, const Vector2& inButtonSize, const Vector2& inFrameSize, uint16_t inButtonHandle, uint16_t inTextHandle, uint16_t inFrameHandle);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

private:
	// フレームのイージング
	void FrameEasing();
#pragma endregion

#pragma region セッター関数
public:
	static void SetCollisionManager2D(CollisionManager2D* inst) { stColMgr2D_ = inst; }

	// 衝突判定の[ON][OFF]を切り替える
	void SetIsCollision(bool frag);
#pragma endregion
};