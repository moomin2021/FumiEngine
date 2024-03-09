#pragma once
#include "Sprite.h"
#include "Player.h"

#include <memory>

class PlayerUI
{
#pragma region メンバ変数
private:
	// プレイヤー
	Player* pPlayer_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> hpBarS_ = nullptr;
	std::unique_ptr<Sprite> hpFrameS_ = nullptr;
	std::unique_ptr<Sprite> hpFrameShadowS_ = nullptr;
	std::unique_ptr<Sprite> reloadUIS_ = nullptr;
	std::unique_ptr<Sprite> reloadBackUIS_ = nullptr;
	std::unique_ptr<Sprite> crossHairS_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sMaxBulletUI_ = {};// 最大弾数表示スプライト
	std::vector<std::unique_ptr<Sprite>> sNowBulletUI_ = {};// 残弾数表示スプライト

	// 画像ハンドル
	uint16_t hpBarH_ = 0;
	uint16_t hpFrameH_ = 0;
	uint16_t hpFrameShadowH_ = 0;
	uint16_t reloadUIH_ = 0;
	uint16_t reloadBackUIH_ = 0;
	uint16_t crossHairH_ = 0;
	uint16_t bulletValueDisplayFrameHandle_ = 0;// 残弾数表示UIフレームハンドル
	std::vector<uint16_t> numberHandle_ = {};// 数字ハンドル
	std::unique_ptr<Sprite> sBulletValueDisplayFrame_ = nullptr;

	Vector2 hpBarSize_ = { 434.0f, 34.0f };
#pragma endregion

#pragma region メンバ関数
public:
	PlayerUI() {}
	~PlayerUI();

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
	void HPUI();
#pragma endregion

#pragma region セッター関数
public:
	void SetPlayer(Player* inPlayer) { pPlayer_ = inPlayer; }
#pragma endregion
};