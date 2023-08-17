#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Collision.h"
#include "SphereCollider.h"

#include <memory>

enum BulletType {
	PLAYER,
	ENEMY0,
};

class Bullet {
#pragma region 構造体
private:
	struct Data {
		std::unique_ptr<Object3D> object = nullptr;
		std::unique_ptr<SphereCollider> col = nullptr;
		Vector3 moveVec = {};
		float bulletSpd = 0.0f;
		uint16_t aliveTime = 0;
	};
#pragma endregion

#pragma region メンバ変数
private:
	// データ
	Data data_ = {};

	// 生存フラグ
	bool isAlive_ = true;

	// 生成された時間
	uint64_t generatedTime_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model"> モデル </param>
	/// <param name="type"> 弾の種類 </param>
	/// <param name="iniPos"> 初期位置 </param>
	Bullet(Model* model, BulletType type, const float3& iniPos, const Vector3& moveVec);

	// デストラクタ
	~Bullet();

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

#pragma endregion

#pragma region ゲッター関数
public:
	/// <summary>
	/// 生存フラグを取得
	/// </summary>
	/// <returns> 生存フラグ </returns>
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};