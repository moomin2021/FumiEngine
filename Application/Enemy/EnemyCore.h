#pragma once
#include "Model.h"
#include "Object3D.h"
#include "PointLight.h"
#include "LightGroup.h"

#include <memory>

class EnemyCore
{
#pragma region メンバ変数
private:
	static LightGroup* sLightGroup_;

	// モデル
	static Model* sCoreM_;
	static Model* sCoreFrameM_;
	static Model* sCoreStandM_;

	// オブジェクト
	std::unique_ptr<Object3D> coreO_ = nullptr;
	std::unique_ptr<Object3D> coreFrameO_ = nullptr;
	std::unique_ptr<Object3D> coreStandO_ = nullptr;

	// オブジェクトのオフセット
	Vector3 offset0_ = { 0.0f, 0.0f, 0.0f };
	Vector3 offset1_ = { 0.0f, 1.5f, 0.0f };

	// HP
	int32_t hp_ = 0;
	int32_t maxHP_ = 10;

	// 生存フラグ
	bool isAlive_ = true;

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化処理
	void Initialize(const Vector3& inPos);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突判定時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	EnemyCore() {}
	~EnemyCore();
#pragma endregion

#pragma region セッター関数
	static void SetModel(Model* inCoreM, Model* inCoreFrameM, Model* inCoreStandM)
	{
		sCoreM_ = inCoreM;
		sCoreFrameM_ = inCoreFrameM;
		sCoreStandM_ = inCoreStandM;
	}

	static void SetLightGroup(LightGroup* inLightGroup) { sLightGroup_ = inLightGroup; }
#pragma endregion

#pragma region ゲッター関数
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};