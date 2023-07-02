#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Collision.h"

#include <memory>

class Bullet {
#pragma region メンバ変数
private:
	// 弾のモデル
	static Model* mBullet_;

	// 弾のオブジェクト
	std::unique_ptr<Object3D> oBullet_;

	// 衝突判定用
	SphereCol col_;

	// 弾の移動方向
	Vector3 moveVec_;

	// 弾の速度
	float bulletSpd_;

	// 生存フラグ
	bool isAlive_;

	// 生存時間
	uint16_t aliveCounter_;	// 生成からのカウント
	uint16_t maxCount_;		// 最大カウント
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="moveVec"> 弾の移動方向 </param>
	/// <param name="iniPos"> 初期座標 </param>
	Bullet(const float3& iniPos, const Vector3& moveVec);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
private:
#pragma endregion

#pragma region セッター関数
public:
	/// <summary>
	/// モデルを設定
	/// </summary>
	/// <param name="model"> モデル </param>
	static void SetModel(Model* model) { mBullet_ = model; }
#pragma endregion

#pragma region ゲッター関数
	/// <summary>
	/// 生存フラグを取得
	/// </summary>
	/// <returns> 生存フラグ </returns>
	bool GetIsAlive() { return isAlive_; }

	inline const SphereCol& GetSphereCol() { return col_; }
#pragma endregion
};