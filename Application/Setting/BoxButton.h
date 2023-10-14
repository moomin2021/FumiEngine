#pragma once
#include "CollisionManager2D.h"
#include "BoxCollider.h"
#include "Sprite.h"

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

	// 画像
	uint16_t hButton_ = 0;
	uint16_t hText_ = 0;

	// コライダー
	std::unique_ptr<BoxCollider> cButton_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	BoxButton() {}
	~BoxButton();

	// 初期化処理
	void Initialize(int32_t inTag, const Vector2& inPos, const Vector2& inSize, uint16_t inButtonHandle, uint16_t inTextHandle);

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
	static void SetCollisionManager2D(CollisionManager2D* inst) { stColMgr2D_ = inst; }
#pragma endregion
};