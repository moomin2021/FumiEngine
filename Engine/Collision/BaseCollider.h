#pragma once
#include "Object3D.h"

enum CollisionShapeType {
	SHAPE_UNKNOWN = -1,	// 未設定
	SHAPE_SPHERE,		// 球
	SHAPE_MESH,			// メッシュ
	SHAPE_RAY,			// レイ
};

class BaseCollider {
public:
	friend class CollisionManager;

#pragma region メンバ変数
protected:
	// 形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	// 任意で紐付けるオブジェクト
	Object3D* object_ = nullptr;

	// 衝突判定を取るか
	bool isCollision_ = true;

	// 衝突フラグ
	bool isHit_ = false;

	// 当たり判定属性
	uint16_t attribute_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	// デストラクタ
	virtual ~BaseCollider() = default;

	// 更新処理
	virtual void Update() = 0;

	/// <summary>
	/// オブジェクト3Dと紐付ける
	/// </summary>
	/// <param name="object"> オブジェクト3D </param>
	inline void LinkObject3D(Object3D* object) { object_ = object; }

	/// <summary>
	/// オブジェクト3Dとの紐づけを解除
	/// </summary>
	inline void CutLinkObject3D() { object_ = nullptr; }

	/// <summary>
	/// 当たり判定属性を追加
	/// </summary>
	/// <param name="attribute"> 当たり判定属性 </param>
	inline void AddAttribute(uint16_t attribute) { attribute_ |= attribute; }

	/// <summary>
	/// 当たり判定属性を削除
	/// </summary>
	/// <param name="attribute"> 当たり判定属性 </param>
	inline void RemoveAttribute(uint16_t attribute) { attribute_ &= !attribute; }
#pragma endregion

#pragma region セッター関数
	/// <summary>
	/// 衝突フラグを設定
	/// </summary>
	/// <param name="frag"></param>
	void SetIsHit(bool frag) { isHit_ = frag; }

	/// <summary>
	/// 衝突判定を取るかフラグを設定
	/// </summary>
	/// <param name="frag"> フラグ </param>
	void SetIsCollision(bool frag) { isCollision_ = frag; }

	/// <summary>
	/// 当たり判定属性を設定
	/// </summary>
	/// <param name="attribute"> 当たり判定属性 </param>
	inline void SetAttribute(uint16_t attribute) { attribute_ = attribute; }
#pragma endregion

#pragma region ゲッター関数
public:
	/// <summary>
	/// 形状タイプを取得
	/// </summary>
	/// <returns></returns>
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	/// <summary>
	/// 衝突判定をとるかフラグを取得
	/// </summary>
	/// <returns> フラグ </returns>
	inline bool GetIsCollision() { return isCollision_; }

	/// <summary>
	/// 衝突フラグを取得
	/// </summary>
	/// <returns> 衝突フラグ </returns>
	inline bool GetIsHit() { return isHit_; }
#pragma endregion
};