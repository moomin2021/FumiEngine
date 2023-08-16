#pragma once
#include "Key.h"
#include "Mouse.h"
#include "Object3D.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include <memory>

class Player
{
#pragma region メンバ変数
private:
	// インスタンス
	Key*	key_	= nullptr;// キーボード
	Mouse*	mouse_	= nullptr;// マウス
	CollisionManager* colMgr_ = nullptr;// コリジョンマネージャー

	// 視点カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;

	// コライダー
	std::unique_ptr<SphereCollider> playerCol_ = nullptr;// プレイヤーのコライダー

	// 移動速度関連
	float moveSpd_		= 0.0f;// 移動速度
	float maxMoveSpd_	= 1.0f;// 最大移動速度
	float moveAcc_		= 0.1f;// 移動加速度

	// カメラ感度
	float sencivity_ = 0.1f;

	// 前方ベクトル
	Vector3 forwardVec_ = { 0.0f, 0.0f, 0.0f };
	// 右方向ベクトル
	Vector3 rightVec_ = { 0.0f, 0.0f, 0.0f };
#pragma endregion

#pragma region メンバ関数
public:
	// デストラクタ
	~Player();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// オブジェクト更新処理
	void ObjUpdate();

	// 衝突時処理
	void OnCollision();
private:
	void Move();
	void EyeMove();
#pragma endregion
};