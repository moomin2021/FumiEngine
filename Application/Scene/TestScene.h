#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Key.h"

#include "ParticleEmitter.h"

#include <memory>

class TestScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	LightGroup* lightGroup_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Object3D> object_ = nullptr;

	// パーティクルエミッター
	std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;
	std::unique_ptr<ParticleEmitter> smokeP_ = nullptr;
	uint16_t particleHandle_ = 0;
	uint16_t smokeHandle_ = 0;
	Vector3 pos_ = { 0.0f, 1.75f, 0.0f };
	float speed_ = 0.1f;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	TestScene();

	// デストラクタ
	~TestScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// デバック
	void Debug();

private:
	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	// パーティクルの生成
	void CreateParticle(uint16_t i);
	void CreateSmoke();
#pragma endregion
};
