#include "Scene3.h"
#include "Texture.h"
#include "Vector3.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "PipelineManager.h"

#include <DirectXMath.h>

using namespace DirectX;

Scene3::Scene3() :
	key_(nullptr)
{
}

Scene3::~Scene3()
{

}

void Scene3::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// �J������ݒ�
	Object3D::SetCamera(camera_.get());

	// ���C�g����
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();

	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });

	lightGroup_->AddDirLight(dirLight_.get());

	// ���C�g��ݒ�
	Object3D::SetLightGroup(lightGroup_.get());

	model_ = std::make_unique<Model>("cube");
	object_ = std::make_unique<Object3D>(model_.get());
}

void Scene3::Update()
{
	// �J�����ړ�
	{
		static float3 eye = { 0.0f, 10.0f, -30.0f };

		eye.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		eye.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;

		camera_->SetEye(eye);
	}

	object_->Update();

	// �J�����̍X�V
	camera_->Update();

	// �Փ˔���
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene3::Draw()
{
	PipelineManager::PreDraw("Object3D");

	object_->Draw();
}