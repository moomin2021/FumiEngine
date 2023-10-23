#pragma once
#include "Sprite.h"
#include "Mouse.h"
#include "CollisionManager2D.h"
#include "PointCollider.h"

#include "ButtonAttribute.h"

#include <memory>

enum class SelectNum {
	NONE = -1,
	START,
	SETTING,
	END,

	GAMEPLAY,
	AUDIO,
};

class HitFrame
{
#pragma region メンバ変数
private:
	// インスタンス
	Mouse* mouse_ = nullptr;
	CollisionManager2D* colMgr2D_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// テクスチャハンドル
	uint16_t handle_ = 0;

	// マウスのコライダー
	std::unique_ptr<PointCollider> cMouse_ = nullptr;

	// イージング関連
	Vector2 size_ = { 0.0f, 0.0f };// 画像のサイズ
	float maxSizeRate_ = 1.1f;// 画像の最大拡大率
	float easeTime_ = 0.5f;// イージング時間
	uint64_t startEaseTime_ = 0;// イージング開始時間

	// 描画フラグ
	bool isDraw_ = false;

	// ボタンの属性
	SelectNum buttonAttr_ = SelectNum::NONE;
#pragma endregion

#pragma region メンバ関数
public:
	HitFrame() {}
	~HitFrame();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突判定時処理
	void OnCollision(SelectNum& selectNum);

	// 行列更新処理
	void MatUpdate();

private:
	// フレームのイージング処理
	void FrameEasing();

	// 前フレームと今の属性が違っていたら[真]を返す
	bool AttrBeforeAndNowDiffer();
#pragma endregion

#pragma region セッター関数
public:
	// フレームを設定
	void SetFrame(const Vector2 inSize, uint16_t frameHandle);

	// 衝突判定の[ON][OFF]を切り替える
	void SetIsCollision(bool frag);
#pragma endregion
};