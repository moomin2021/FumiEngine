#pragma once
#include "Key.h"
#include "Mouse.h"
#include "CollisionManager.h"
#include "RayCollider.h"
#include "SphereCollider.h"
#include "Camera.h"
#include "Sprite.h"
#include "Sound.h"

#include "Bullet.h"
#include "ItemManager.h"

#include <string>
#include <vector>
#include <memory>
#include <deque>

class EnemyManager;

class Player
{
	enum State {
		NORMAL,	// 通常状態
		AIR,	// 空中状態
		CLIMB,	// 登り状態
	};

	std::vector<std::string> stateName_ = {
		"NORMAL",
		"AIR",
		"CLIMB"
	};

#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;
	CollisionManager* colMgr_ = nullptr;
	EnemyManager* enemyMgr_ = nullptr;
	Sound* sound_ = nullptr;

	// 状態
	State state_ = NORMAL;

	// サウンド
	uint16_t shotSE_ = 0;

	// カメラ関連
	std::unique_ptr<Camera> camera_ = nullptr;// カメラ本体
	float cameraHeight_ = 1.62f;
	float sencivity_ = 0.1f;// カメラ感度
	Vector3 eyeAngle_	= { 0.0f, 90.0f, 0.0f };// カメラ角度
	Vector3 forwardVec_	= { 0.0f, 0.0f, 0.0f };// 正面ベクトル
	Vector3 rightVec_	= { 0.0f, 0.0f, 0.0f };// 右ベクトル
	float fovAngleY_ = 70.0f;
	float diffusivity_ = 5.0f;
	const float maxDiffusivity_ = 5.0f;
	float adsRate_ = 0.0f;
	bool isAds_ = false;
	float recoilEyeAngle_ = 0.0f;

	// モデル
	std::unique_ptr<Model> mSphere_ = nullptr;// 球
	std::unique_ptr<Model> mSheriff_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3D> oPlayer_ = nullptr;// プレイヤー
	std::unique_ptr<Object3D> oSheriff_ = nullptr;

	// コライダー
	std::unique_ptr<AABBCollider> playerCol_ = nullptr;// プレイヤーコライダー
	std::unique_ptr<RayCollider> legCol_ = nullptr;// 足元コライダー
	std::unique_ptr<RayCollider> climbCol_ = nullptr;// 壁登りに使うコライダー
	std::unique_ptr<RayCollider> shotCol_ = nullptr;// 弾を撃った時に使うコライダー
	std::unique_ptr<RayCollider> eyeCol_ = nullptr;// 視線コライダー

	// 移動関連
	float moveSpd_ = 0.0f;// 移動速度
	float maxMoveSpd_ = 0.15f;// 最大移動速度
	float moveAcc_ = 0.05f;// 移動加速度
	bool isDash_ = false;// ダッシュフラグ
	float dashSpd_ = 0.3f;// ダッシュ速度

	// ジャンプ関連
	float gravity_ = 0.0f;// 重力
	float maxGravity_ = 0.5f;// 最大重力
	float gAcc_ = 0.1f;// 重力加速度
	float jumpSpd_ = 0.5f;// ジャンプ速度

	// クロスヘア
	uint16_t crossHairHandle_ = 0;
	std::unique_ptr<Sprite> sCrossHair_ = nullptr;

	// 弾
	std::unique_ptr<Model> mBullet_ = nullptr;
	std::deque<std::unique_ptr<Bullet>> bullets_ = {};
	uint8_t maxBullet_ = 99;// 最大弾数
	uint8_t nowBullet_ = 99;// 現在弾数
	uint16_t bulletValueDisplayFrameHandle_ = 0;// 残弾数表示UIフレームハンドル
	std::vector<uint16_t> numberHandle_ = {};// 数字ハンドル
	std::unique_ptr<Sprite> sBulletValueDisplayFrame_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sMaxBulletUI_ = {};// 最大弾数表示スプライト
	std::vector<std::unique_ptr<Sprite>> sNowBulletUI_ = {};// 残弾数表示スプライト
	float shotInterval_ = 0.5f;
	uint64_t shotTime_ = 0;

	// リロード
	bool isReload_ = false;	// リロードしているか
	uint8_t reloadTime_ = 3;// リロード時間
	uint16_t reloadUIHandle_ = 0;// リロードUIハンドル
	std::unique_ptr<Sprite> sReloadUI_ = nullptr;// リロードUIスプライト

#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 3D描画処理
	void Draw3D();

	// 前面2D描画処理
	void DrawFront2D();

	// 衝突判定時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	// デバック
	void Debug();

private:
	// 状態別処理
	static void (Player::* stateTable[]) ();
	void Normal();	// 通常状態
	void Air();		// 空中状態
	void Climb();	// 登り状態

	// 行動関数
	void Ads();		// 覗き込み
	void Shoot();	// 弾を撃つ
	void Reload();	// リロード処理
	void Move();	// 移動操作
	void EyeMove();	// 視点操作
	void Jump();	// ジャンプ処理
	void Fall();	// 落下処理
	void Dash();	// 走行処理
#pragma endregion

#pragma region セッター関数
	public:
	void SetEnemyManager(EnemyManager* inst) { enemyMgr_ = inst; }
#pragma endregion

#pragma region ゲッター関数
public:
	inline const Vector3& GetPosition() { return camera_->GetEye(); }
	const Vector3& GetDir() { return forwardVec_; }
#pragma endregion
};
