#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include <memory>

class Boss0
{
#pragma region メンバ変数
private:
	// インスタンス
	CollisionManager* colMgr_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;

	// コライダー
	std::unique_ptr<SphereCollider> collider_ = nullptr;

	// HP
	uint16_t hp_ = 100;

	uint16_t damageCounter_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Boss0();

	// デストラクタ
	~Boss0();

	// 初期化処理
	void Initialize(const Vector3& pos);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// オブジェクト更新
	void ObjUpdate();

	// 衝突時処理
	void OnCollision();
#pragma endregion
};