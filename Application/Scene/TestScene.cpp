#include "TestScene.h"
#include "CollisionManager3D.h"
#include "WinAPI.h"
#include "Texture.h"

#include "PipelineManager.h"

#include <set>
#include <imgui_impl_DX12.h>

TestScene::TestScene(IScene* sceneIf) : BaseScene(sceneIf) {}

void TestScene::Initialize()
{
#pragma region カーソルの設定
	WinAPI::GetInstance()->DisplayCursor(false);
	WinAPI::GetInstance()->SetClipCursor(true);
#pragma endregion

#pragma region インスタンス
	key_ = Key::GetInstance();
	lightGroup_ = LightGroup::GetInstance();
	colMgr3D_ = CollisionManager3D::GetInstance();
#pragma endregion

#pragma region ライトグループ
	lightGroup_ = LightGroup::GetInstance();
	Object3D::SetLightGroup(lightGroup_);

	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -10.0f });
	camera_->SetTarget({ 0.0f, 0.0f, 0.0f });
	Object3D::SetCamera(camera_.get());
#pragma endregion

	model_ = std::make_unique<Model>("stoneBrick");

	obj0_ = std::make_unique<Object3D>(model_.get());
	obj1_ = std::make_unique<Object3D>(model_.get());
	obj2_ = std::make_unique<Object3D>(model_.get());
	obj0_->SetPosition(Vector3(2.0f, 0.0f, 0.0f));
	obj1_->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	obj2_->SetPosition(Vector3(-2.0f, 0.0f, 0.0f));

	col0_ = std::make_unique<AABBCollider>();
	col1_ = std::make_unique<AABBCollider>();
	col2_ = std::make_unique<AABBCollider>();

	col0_->SetAttribute(1);
	col0_->SetRadius(Vector3(0.5f, 0.5f, 0.5f));
	col0_->SetObject3D(obj0_.get());
	col0_->SetPushBackRate(1.0f);
	col1_->SetAttribute(1);
	col1_->SetRadius(Vector3(0.5f, 0.5f, 0.5f));
	col1_->SetObject3D(obj1_.get());
	col1_->SetPushBackRate(1.0f);
	col2_->SetAttribute(1);
	col2_->SetRadius(Vector3(0.5f, 0.5f, 0.5f));
	col2_->SetObject3D(obj2_.get());
	col2_->SetPushBackRate(1.0f);

	colMgr3D_->AddCollider(col0_.get());
	colMgr3D_->AddCollider(col1_.get());
	colMgr3D_->AddBlockCollider(col2_.get());
}

void TestScene::Update()
{
	static Vector3 pos = { 2.0f, 0.0f,0.0f };

	pos.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.05f;
	pos.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.05f;
	obj0_->SetPosition(pos);

	// 衝突時処理
	Collision();

	// 行列更新処理
	MatUpdate();

	// デバック
	Debug();
}

void TestScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	obj0_->Draw();
	obj1_->Draw();
	obj2_->Draw();
}

void TestScene::Finalize()
{
	lightGroup_->RemoveDirLight(dirLight_.get());
}

void TestScene::Debug()
{

}

void TestScene::Collision()
{
	// 衝突判定をとる
	CollisionManager3D::GetInstance()->CheckAllCollision();
}

void TestScene::MatUpdate()
{
	camera_->Update();
	obj0_->MatUpdate();
	obj1_->MatUpdate();
	obj2_->MatUpdate();
}
