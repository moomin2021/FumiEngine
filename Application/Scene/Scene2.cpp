#include "Scene2.h"
#include "Texture.h"
#include "Vector3.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include <DirectXMath.h>

using namespace DirectX;

Scene2::Scene2() :
	key_(nullptr)
{
}

Scene2::~Scene2()
{

}

void Scene2::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// カメラを設定
	Object3D::SetCamera(camera_.get());

	// ライト生成
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();

	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });

	lightGroup_->AddDirLight(dirLight_.get());

	// ライトを設定
	Object3D::SetLightGroup(lightGroup_.get());

	// ディゾルブを生成
	dissolve_ = std::make_unique<Dissolve3D>();
	dissolve_->Initialize();
}

void Scene2::Update()
{
	// カメラ移動
	{
		static float3 eye = { 0.0f, 10.0f, -30.0f };

		eye.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		eye.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;

		camera_->SetEye(eye);
	}

	static float t = 0.0f;

	if (key_->PushKey(DIK_UP)) t += 0.01f;
	if (key_->PushKey(DIK_DOWN)) t -= 0.01f;

	t = Util::Clamp(t, 1.01f, 0.0f);

	dissolve_->SetDissolveTime(t);

	dissolve_->Update();

	// カメラの更新
	camera_->Update();

	// 衝突判定
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene2::Draw()
{
	dissolve_->Draw();
}