#include "Scene4.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>

Scene4::Scene4() :
	key_(nullptr),
	camera_(nullptr),
	lightGroup_(nullptr),
	sphereM_(nullptr),
	rayM_(nullptr),
	triangleM_(nullptr),
	object_{},
	rayC_{},
	triangleC_{},
	count_(0)
{
}

Scene4::~Scene4()
{
	delete camera_;
	delete lightGroup_;
	delete sphereM_;
	delete rayM_;
	delete triangleM_;
	delete object_[0];
	delete object_[1];
	delete object_[2];
}

void Scene4::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// モデル
	sphereM_ = Model::CreateModel("sphere");
	rayM_ = Model::CreateModel("ray");
	triangleM_ = Model::CreateModel("triangle");

	// オブジェクト
	object_[0] = Object3D::CreateObject3D(sphereM_);
	object_[0]->SetPos({ 0.0f, 5.0f, 0.0f });

	object_[1] = Object3D::CreateObject3D(rayM_);
	object_[1]->SetPos({ 0.0f, 5.0f, 0.0f });
	object_[1]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	object_[2] = Object3D::CreateObject3D(triangleM_);
	object_[2]->SetPos({ 0.0f, 0.0f, 0.0f });
	object_[2]->SetRot({ 0.0f, 90.0f, 0.0f });
	object_[2]->SetScale({ 5.0f, 1.0f, 5.0f });

	// ライト生成
	lightGroup_ = LightGroup::Create();

	lightGroup_->SetDirLightActive(0, true);
	lightGroup_->SetDirLightActive(1, false);
	lightGroup_->SetDirLightActive(2, false);

	// カメラを設定
	Object3D::SetCamera(camera_);

	// ライトを設定
	Object3D::SetLightGroup(lightGroup_);

	// レイの初期値を設定
	rayC_.start = XMVectorSet(0.0f, 5.0f, 0.0f, 1.0f);// -> 原点やや上
	rayC_.dir = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);// -> 下向き

	// 三角形の初期値を設定
	triangleC_.p0 = XMVectorSet(-5.0f, 0.0f, -5.0f, 1.0f);// -> 左手前
	triangleC_.p1 = XMVectorSet(-5.0f, 0.0f, 5.0f, 1.0f);// -> 左奥
	triangleC_.p2 = XMVectorSet(5.0f, 0.0f, -5.0f, 1.0f);// -> 右手前
	triangleC_.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);// -> 上向き
}

void Scene4::Update()
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
		Float3 defaPos = { 0.0f, 5.0f, 0.0f };
		defaPos.x += sinf((float)count_ * 0.05f) * 7.0f;
		defaPos.z += sinf((float)count_ * 0.05f) * 7.0f;

		rayC_.start.m128_f32[0] = defaPos.x;
		rayC_.start.m128_f32[2] = defaPos.z;

		object_[0]->SetPos(defaPos);
		object_[1]->SetPos(defaPos);

		float distance = 0.0f;

		if (Collision::CheckRay2Triangle(rayC_, triangleC_, &distance)) {
			object_[0]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		}
		else {
			object_[0]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}

		object_[1]->SetScale({ 1.0f, distance / 2.0f, 1.0f });
	}

	// カメラの更新
	camera_->Update();

	// ライトの更新
	lightGroup_->Update();
}

void Scene4::Draw()
{
	Object3D::PreDraw();

	for (auto& object : object_) object->Draw();
}