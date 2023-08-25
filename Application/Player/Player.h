#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Object3D.h"
#include "SphereCollider.h"
#include "RayCollider.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "Bullet.h"
#include "Sprite.h"

#include <memory>
#include <deque>

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
	Key*	key_	= nullptr;// キーボード
	Mouse*	mouse_	= nullptr;// マウス
	CollisionManager* colMgr_ = nullptr;// コリジョンマネージャー
	EnemyManager* enemyMgr_ = nullptr;

	// 視点カメラ
	std::unique_ptr<Camera> camera_ = nullptr;
	float fovAngleY_ = 70.0f;

	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Model> mSheriff_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;
	std::unique_ptr<Object3D> testObj_ = nullptr;
	std::unique_ptr<Object3D> oSheriff_ = nullptr;

	// コライダー
	std::unique_ptr<SphereCollider> playerCol_ = nullptr;// プレイヤーのコライダー
	std::unique_ptr<RayCollider> legCol_ = nullptr;// 足元のコライダー(落下処理に使用)
	std::unique_ptr<RayCollider> climbCol_ = nullptr;// 壁登りに使うコライダー
	std::unique_ptr<RayCollider> eyeCol_ = nullptr;// 視点コライダー

	// 状態
	State state_ = NORMAL;

	// カメラ感度
	float sencivity_ = 0.1f;

	// 視点角度
	Vector3 eyeAngle_ = { 0.0f, 90.0f, 0.0f };
	float diffusivity_ = 5.0f;
	const float maxDiffusivity_ = 5.0f;
	float adsRate_ = 0.0f;
	bool isAds_ = false;

	// 前方ベクトル
	Vector3 forwardVec_ = { 0.0f, 0.0f, 0.0f };

	// 右方向ベクトル
	Vector3 rightVec_ = { 0.0f, 0.0f, 0.0f };

	// 移動速度関連
	float moveSpd_		= 0.0f;// 移動速度
	float maxMoveSpd_	= 1.0f;// 最大移動速度
	float moveAcc_		= 0.1f;// 移動加速度

	// クロスヘア
	uint16_t crossHairHandle_ = 0;
	std::unique_ptr<Sprite> sCrossHair_ = nullptr;

	// 弾
	std::deque<std::unique_ptr<Bullet>> bullets_;
	std::unique_ptr<Model> mBullet_ = nullptr;// 弾のモデル
	uint8_t maxBullet_ = 30;// 最大弾数
	uint8_t nowBullet_ = 30;// 現在弾数
	uint16_t bulletValueDisplayFrameHandle_ = 0;// 残弾数表示UIハンドル
	std::unique_ptr<Sprite> sBulletValueDisplayFrame_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sMaxBulletUI_;// 最大弾数表示スプライト
	std::vector<std::unique_ptr<Sprite>> sNowBulletUI_;// 残弾数表示スプライト
	float shotInterval_ = 0.1f;// 撃つインターバル
	uint64_t shotTime_ = 0;// 弾を撃った時間
	std::vector<uint16_t> numberHandle_;// 数字

	// リロード
	bool isReload_ = false;	// リロードしているか
	uint8_t reloadTime_ = 3;// リロード時間
	uint16_t reloadUIHandle_ = 0;// リロードUIハンドル
	std::unique_ptr<Sprite> sReloadUI_ = nullptr;// リロードUIスプライト

	// ジャンプ用
	bool isGround_ = true;// 地面についているか
	float gravity_		= 0.0f;// 重力
	float maxGravity_	= 1.5f;// 最大重力
	float gAcc_			= 0.2f;// 重力加速度
	float jumpSpd_		= 1.5f;// ジャンプ速度

	// ダッシュ
	bool isDash_ = false;
	float dashSpd_ = 2.0f;
#pragma endregion

#pragma region メンバ関数
public:
	// デストラクタ
	~Player();

	// 初期化処理
	void Initialize(EnemyManager* enemyMgr);

	// 更新処理
	void Update();

	// 描画処理
	void DrawObject3D();
	void DrawSprite();

	// オブジェクト更新処理
	void ObjUpdate();

	// 衝突時処理
	void OnCollision();
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
};