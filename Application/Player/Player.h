#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Camera.h"
#include "Bullet.h"
#include "Sprite.h"
#include "RayCollider.h"

#include <memory>

class Player
{
	enum State {
		NORMAL,	// 通常状態
		AIR,	// 空中状態
	};

#pragma region メンバ変数
public:
	// 弾のクラス
	std::vector <std::unique_ptr<Bullet>> bullets_;

private:
	// 入力クラスインスタンス
	Key* key_		= nullptr;// キーボード
	Mouse* mouse_	= nullptr;// マウス

	// 視点カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// 数字
	std::vector<uint16_t> numberHandle_;

	// 弾
	std::unique_ptr<Model> mBullet_ = nullptr;// 弾のモデル
	uint8_t maxBullet_ = 30;// 最大弾数
	uint8_t nowBullet_ = 30;// 現在弾数
	uint16_t bulletValueDisplayFrameHandle_ = 0;// 残弾数表示UIハンドル
	std::unique_ptr<Sprite> sBulletValueDisplayFrame_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sMaxBulletUI_;// 最大弾数表示スプライト
	std::vector<std::unique_ptr<Sprite>> sNowBulletUI_;// 残弾数表示スプライト

	// リロード
	bool isReload_ = false;	// リロードしているか
	uint8_t reloadTime_ = 3;// リロード時間
	uint16_t reloadUIHandle_ = 0;// リロードUIハンドル
	std::unique_ptr<Sprite> sReloadUI_ = nullptr;// リロードUIスプライト

	// レイのコライダー
	std::unique_ptr<RayCollider> eyeCollider_ = nullptr;// 視点のレイ

	// 状態
	State state_ = NORMAL;

	// 方向ベクトル
	Vector3 forwardVec_ = {};// 前方
	Vector3 rightVec_	= {};// 右

	// 速度
	float moveAcc_			= 0.1f;// 移動加速度
	float maxSpd_			= 1.5f;// 最大速度
	float moveSpd_			= 0.0f;// 移動速度
	float cameraAngleSpd_	= 0.1f;// カメラの角度の移動速度

	// クロスヘア
	uint16_t crossHairHandle_ = 0;
	std::unique_ptr<Sprite> sCrossHair_ = nullptr;

	// ジャンプ用
	bool isGround_	= true;// 地面についているか
	float gravity_	= 0.0f;	// 重力
	float gAcc_		= 0.2f;	// 重力加速度
	float jumpSpd_	= 2.5f;	// ジャンプ速度
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 更新処理
	void Update();

	// オブジェクト3D描画処理
	void Object3DDraw();

	// 前面スプライト描画
	void FrontSpriteDraw();

private:
	// 状態別処理
	static void (Player::* stateTable[]) ();
	void Normal();	// 通常状態
	void Air();		// 空中状態

	// 行動関数
	void Shoot();	// 弾を撃つ
	void EyeMove();	// 視点操作
	void Move();	// 移動操作
	void Jump();	// ジャンプ処理
	void Fall();	// 落下処理
	void Reload();	// リロード処理

	// コライダーの更新
	void ColliderUpdate();

	// 衝突時の処理
	void OnCollision();
#pragma endregion

#pragma region ゲッター関数

#pragma endregion
};