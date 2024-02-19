#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Key.h"
#include "Mouse.h"
#include "Matrix4.h"

#include "ParticleEmitter.h"

#include <memory>

class TestScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;
	LightGroup* lightGroup_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Object3D> object_ = nullptr;

	Matrix4 screen_ = {};
	Vector3 result0_ = Vector3();
	Vector3 result1_ = Vector3();

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
#pragma endregion
};
