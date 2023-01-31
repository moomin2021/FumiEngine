#include "Scene1.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>

Scene1::Scene1() :
	key_(nullptr),
	camera_(nullptr),
	lightGroup_(nullptr),
	sphereM_(nullptr),
	object_{},
	sphereC_{},
	planeC_{},
	count_(0)
{
}

Scene1::~Scene1()
{
	delete camera_;
	delete lightGroup_;
	delete sphereM_;
	delete floorM_;
	delete object_[0];
	delete object_[1];
}

void Scene1::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 2.0f, -30.0f };

	// モデル
	sphereM_ = Model::CreateModel("sphere");
	floorM_ = Model::CreateModel("floor");

	// オブジェクト
	object_[0] = Object3D::CreateObject3D(sphereM_);
	object_[0]->SetPos({ 0.0f, 1.0f, 0.0f });

	object_[1] = Object3D::CreateObject3D(floorM_);
	object_[1]->SetPos({ 0.0f, 0.0f, 0.0f });
	object_[1]->SetScale({ 10.0f, 10.0f, 10.0f });

	// ライト生成
	lightGroup_ = LightGroup::Create();

	lightGroup_->SetDirLightActive(0, true);
	lightGroup_->SetDirLightActive(1, false);
	lightGroup_->SetDirLightActive(2, false);

	// カメラを設定
	Object3D::SetCamera(camera_);

	// ライトを設定
	Object3D::SetLightGroup(lightGroup_);

	// 弾の初期値を設定
	sphereC_.center = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);// -> 中心点座標
	sphereC_.radius = 1.0f;// -> 半径

	// 平面の初期値を設定
	planeC_.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);// -> 法線ベクトル
	planeC_.distance = 0.0f;// -> 原点(0.0f, 0.0f, 0.0f)からの距離
}

void Scene1::Update()
{
	if (key_->TriggerKey(DIK_0)) {
		int num = 0;
	}

	// カウント進行
	count_++;

	// カメラ移動
	{
		camera_->eye_.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		camera_->eye_.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;
	}

	// 球移動
	{
		Float3 defaPos = { 0.0f, 1.0f, 0.0f };
		defaPos.y += sinf((float)count_ * 0.05f) * 3.0f;

		sphereC_.center.m128_f32[1] = defaPos.y;

		object_[0]->SetPos(defaPos);

		if (Collision::CheckSphere2Plane(sphereC_, planeC_)) {
			object_[0]->SetColor({1.0f, 0.0f, 0.0f, 1.0f});
		}
		else {
			object_[0]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}

	// カメラの更新
	camera_->Update();

	// ライトの更新
	lightGroup_->Update();
}

void Scene1::Draw()
{
	Object3D::PreDraw();

	for (auto& object : object_) object->Draw();
}