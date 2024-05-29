#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Key.h"
#include "CollisionManager3D.h"

#include "AABBCollider.h"

#include <memory>

class TestScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;
	LightGroup* lightGroup_ = nullptr;
	CollisionManager3D* colMgr3D_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;

	std::unique_ptr<Object3D> obj0_ = nullptr;
	std::unique_ptr<Object3D> obj1_ = nullptr;
	std::unique_ptr<Object3D> obj2_ = nullptr;

	std::unique_ptr<AABBCollider> col0_ = nullptr;
	std::unique_ptr<AABBCollider> col1_ = nullptr;
	std::unique_ptr<AABBCollider> col2_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタとデストラクタ
	TestScene(IScene* sceneIf);
	~TestScene() {}

	// 基本処理
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	// デバック
	void Debug();

private:
	// 衝突時処理
	void Collision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion
};
