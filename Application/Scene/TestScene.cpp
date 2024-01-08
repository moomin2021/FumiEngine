#include "TestScene.h"
#include "WinAPI.h"
#include "Texture.h"

#include "PipelineManager.h"
#include "CollisionAttribute.h"

#include <set>
#include <imgui_impl_DX12.h>

TestScene::TestScene() {}

TestScene::~TestScene()
{
	lightGroup_->RemoveDirLight(dirLight_.get());
}

void TestScene::Initialize()
{
#pragma region カーソルの設定
	WinAPI::GetInstance()->DisplayCursor(false);
	WinAPI::GetInstance()->SetClipCursor(true);
#pragma endregion

#pragma region インスタンス
	key_ = Key::GetInstance();
	lightGroup_ = LightGroup::GetInstance();
	colMgr_ = CollisionManager::GetInstance();
	Object3D::SetLightGroup(lightGroup_);
	Instancing3D::SetLightGroup(lightGroup_);
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -10.0f });
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ライトグループ
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region モデル
	model_ = std::make_unique<Model>("stoneBrick");
#pragma endregion

#pragma region オブジェクト
	objects_.resize(3);

	objects_[0] = std::make_unique<Object3D>(model_.get());
	objects_[0]->SetPosition({ -1.1f, 0.0f, 0.0f });
	objects_[0]->SetScale({ 2.0f, 1.0f, 1.0f });

	objects_[1] = std::make_unique<Object3D>(model_.get());
	objects_[1]->SetPosition({ 1.1f, 0.0f, 0.0f });
	objects_[1]->SetScale({ 2.0f, 1.0f, 1.0f });

	objects_[2] = std::make_unique<Object3D>(model_.get());
	objects_[2]->SetPosition({ 0.0f, 0.0f, -2.0f });
#pragma endregion

#pragma region コライダー
	colliders_.resize(3);

	colliders_[0] = std::make_unique<AABBCollider>();
	colliders_[0]->SetAttribute(COL_STAGE_OBJ);
	colliders_[0]->SetOffset({ 0.0f, 0.0f, 0.0f });
	colliders_[0]->SetRadius({ 1.0f, 0.5f, 0.5f });
	colliders_[0]->SetObject3D(objects_[0].get());
	colMgr_->AddBlockCollider(colliders_[0].get());

	colliders_[1] = std::make_unique<AABBCollider>();
	colliders_[1]->SetAttribute(COL_STAGE_OBJ);
	colliders_[1]->SetOffset({ 0.0f, 0.0f, 0.0f });
	colliders_[1]->SetRadius({ 1.0f, 0.5f, 0.5f });
	colliders_[1]->SetObject3D(objects_[1].get());
	colMgr_->AddBlockCollider(colliders_[1].get());

	colliders_[2] = std::make_unique<AABBCollider>();
	colliders_[2]->SetAttribute(COL_STAGE_OBJ);
	colliders_[2]->SetOffset({ 0.0f, 0.0f, 0.0f });
	colliders_[2]->SetRadius({ 0.5f, 0.5f, 0.5f });
	colliders_[2]->SetObject3D(objects_[2].get());
	colMgr_->AddCollider(colliders_[2].get());
#pragma endregion
}

void TestScene::Update()
{
	camera_->Update();

	Vector3 pos = objects_[2]->GetPosition();

	pos.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.1f;
	pos.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.1f;

	objects_[2]->SetPosition(pos);

	OnCollision();
	MatUpdate();
}

void TestScene::Draw()
{
	PipelineManager::PreDraw("Object3D");
	for (auto& it : objects_) it->Draw();
}

void TestScene::Debug()
{

}

void TestScene::OnCollision()
{
	// 衝突判定をとる
	CollisionManager::GetInstance()->CheckAllCollision();
}

void TestScene::MatUpdate()
{
	for (auto& it : objects_) it->MatUpdate();
}
