#pragma once
#include "CollisionManager2D.h"
#include "BoxCollider.h"
#include "Sprite.h"

#include "ButtonAttribute.h"

#include <memory>

class ResultBox
{
#pragma region メンバ変数
private:
	// インスタンス
	static CollisionManager2D* sColMgr2D_;

	// スプライト
	std::unique_ptr<Sprite> boxS_ = nullptr;
	std::unique_ptr<Sprite> textS_ = nullptr;

	// 画像
	uint16_t boxH_ = 0;
	uint16_t textH_ = 0;

	// コライダー
	std::unique_ptr<BoxCollider> boxC_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	ResultBox() {}
	~ResultBox();

	// 初期化処理
	void Initialize(const Vector2& inPos, const Vector2& inSize,
		uint16_t inBoxHandle, uint16_t inTextHandle);

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
	static void SetCollisionManager2D(CollisionManager2D* inst) { sColMgr2D_ = inst; }
#pragma endregion
};