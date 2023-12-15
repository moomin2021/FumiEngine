#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Object3D.h"
#include "Model.h"
#include "Camera.h"

#include "CollisionManager.h"
#include "AABBCollider.h"
#include "RayCollider.h"

#include <memory>

class EnemyManager;

class Player
{
	enum State : uint8_t {
		NORMAL,
		AIR,
	};

	std::vector<std::string> stateName_ = {
		"NORMAL",
		"AIR",
	};

#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;
	CollisionManager* colMgr_ = nullptr;

	// モデル
	std::unique_ptr<Model> playerM_ = nullptr;
	std::unique_ptr<Model> sheriffM_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3D> playerO_ = nullptr;// プレイヤー
	std::unique_ptr<Object3D> sheriffO_ = nullptr;// ピストル

	// コライダー
	std::unique_ptr<AABBCollider> playerC_ = nullptr;// プレイヤー
	std::unique_ptr<RayCollider> groundJudgmentC_ = nullptr;// 接地判定
	std::unique_ptr<RayCollider> shotC_ = nullptr;// 撃った時に飛ばすレイ

	// 状態
	State state_ = State::NORMAL;
	uint8_t hp_ = 5;

	// カメラ関連
	std::unique_ptr<Camera> camera_ = nullptr;
	float cameraHeight_ = 1.62f;// カメラ(目線)の高さ
	float sencivity_ = 0.1f;// カメラ感度
	Vector3 eyeAngle_ = { 0.0f, 90.0f, 0.0f };// カメラ角度
	Vector3 forwardVec_ = { 0.0f, 0.0f, 0.0f };// 正面ベクトル
	Vector3 rightVec_ = { 0.0f, 0.0f, 0.0f };// 右ベクトル
	const float maxFovAngle_ = 90.0f;// 最大視野角
	const float minFovAngle_ = 40.0f;// 最小視野角
	const float defFovAngle_ = 70.0f;// デフォルトの視野角
	float nowFovAngle_ = 70.0f;// 現在の視野角
	float fovAngleIncrDecValue_ = 2.0f;// 視野角の増減値
	const float maxDiffusivity_ = 5.0f;// 最大拡散
	float nowDiffusivity_ = 5.0f;// 現在の拡散
	float adsRate_ = 0.0f;// ADS率(1.0で完全にADSしている状態になる)
	float adsRateIncrDecValue_ = 0.1f;// ADS率の増減値
	bool isAds_ = false;// ADSしているかフラグ
	const float maxRecoilEyeAngle_ = 10.0f;// 最大リコイル角度
	float nowRecoilEyeAngle_ = 0.0f;// リコイル角度
	float decRecoilEyeAngle_ = 0.5f;// リコイル角度の減少値

	// 移動関連
	float moveSpd_ = 0.0f;// 移動速度
	float maxMoveSpd_ = 0.15f;// 最大移動速度
	float moveAcc_ = 0.05f;// 移動加速度
	bool isDash_ = false;// ダッシュフラグ
	float dashSpd_ = 0.3f;// ダッシュ速度
	Vector3 knockVec_ = { 0.0f, 0.0f, 0.0f };

	// ジャンプ関連
	float gravity_ = 0.0f;// 重力
	float maxGravity_ = 0.5f;// 最大重力
	float gAcc_ = 0.1f;// 重力加速度
	float jumpSpd_ = 0.5f;// ジャンプ速度

	// 射撃関連
	uint8_t maxBullet_ = 99;// 最大弾数
	uint8_t nowBullet_ = 99;// 現在弾数
	float shotInterval_ = 0.5f;// 射撃の間隔
	uint64_t shotTime_ = 0;// 最後に撃った時間
	bool isReload_ = false;	// リロードしているか
	uint8_t reloadTime_ = 3;// リロード時間
#pragma endregion

#pragma region メンバ関数
public:
	Player() {}
	~Player();

	// 初期化処理
	void Initialize(const Vector3& startPos);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時更新処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	// デバック
	void Debug();

	// ノックバック
	void SetKnock(const Vector3& vec);

	// シーン切り替え
	void CheckSceneChange();

public:
	// 状態別処理
	static void (Player::*state[]) ();
	void Normal();
	void Air();

	// 行動関数
	void EyeMove();
	void Move();
	void Jump();
	void Fall();
	void Dash();
	void Ads();
	void Shot();
	void Reload();

	void Recoil();// リコイル計算
	void Sheriff();// ピストルのオブジェクト位置計算
	void GroundingJudgment();// 接地判定
#pragma endregion

#pragma region セッター関数
public:
#pragma endregion

#pragma region ゲッター関数
public:
	inline const Vector3& GetPosition() { return playerO_->GetPosition(); }
	inline const Vector3& GetDir() { return forwardVec_; }
#pragma endregion
};
