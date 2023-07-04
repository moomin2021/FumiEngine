#pragma once
#include "CollisionTypes.h"
#include "Object3D.h"
#include "CollisionInfo.h"

class BaseCollider {
#pragma region メンバ変数
protected:
	// オブジェクト3D
	Object3D* object3d_;

	// 形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseCollider() = default;

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseCollider() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info"> 衝突情報 </param>
	inline void OnCollision(const CollisionInfo& info) { object3d_->OnCollision(info); }
#pragma endregion

#pragma region セッター関数
	/// <summary>
	/// オブジェクト3Dを設定
	/// </summary>
	/// <param name="object3d"></param>
	inline void SetObject3D(Object3D* object3d) { object3d_ = object3d; }
#pragma endregion

#pragma region メンバ関数
	/// <summary>
	/// オブジェクト3Dを取得
	/// </summary>
	/// <returns></returns>
	inline Object3D* GetObject3D() { return object3d_; }

	/// <summary>
	/// 形状タイプを取得
	/// </summary>
	/// <returns></returns>
	inline CollisionShapeType GetShapeType() { return shapeType_; }
#pragma endregion
};