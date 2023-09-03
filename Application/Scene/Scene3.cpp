#include "Scene3.h"
#include "Texture.h"
#include "CollisionManager2D.h"
#include "ImGuiManager.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_DX12.h>

Scene3::Scene3() {}

Scene3::~Scene3()
{
	
}

void Scene3::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();
	mouse_ = Mouse::GetInstance();

#pragma region コライダー初期化
	pCol_ = std::make_unique<PointCollider>(Vector2{ 0.0f, 0.0f });
	boxCol_ = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.0f });
#pragma endregion
}

void Scene3::Update()
{
	// 衝突判定
	CollisionManager2D::GetInstance()->CheckAllCollision();
}

void Scene3::Draw()
{

}