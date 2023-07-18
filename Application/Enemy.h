#pragma once
#include "Object3D.h"
#include "Collision.h"

#include <memory>

class Enemy {
#pragma region メンバ変数
private:
	// モデル
	static Model* model_;

	// オブジェクト
	std::unique_ptr<Object3D> obj_;

	// 衝突判定用
	//SphereCol col_;

	// HP
	uint16_t hp_;

	// 生存フラグ
	bool isAlive_;

	uint16_t damageCounter_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// HPを減らす
	/// </summary>
	/// <param name="reduceValue"> 減らす値 </param>
	void ReduceHP(uint16_t reduceValue);
private:

#pragma endregion

#pragma region セッター関数
public:
	/// <summary>
	/// モデルを設定
	/// </summary>
	/// <param name="model"> モデル </param>
	static inline void SetModel(Model* model) { model_ = model; }

	/// <summary>
	/// オブジェクトの座標(XYZ)を設定
	/// </summary>
	/// <param name="pos"> 座標(XYZ) </param>
	inline void SetPosition(const float3& pos) { obj_->SetPosition(pos); }

	/// <summary>
	/// オブジェクトの拡縮(XYZ)を設定
	/// </summary>
	/// <param name="scale"> 拡縮(XYZ) </param>
	inline void SetScale(const float3& scale) { obj_->SetScale(scale); }
#pragma endregion

#pragma region ゲッター関数
	//inline const SphereCol& GetSphereCol() { return col_; }

	inline bool GetIsAlive() { return isAlive_; }
#pragma endregion
};