#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include <DirectXMath.h>

using namespace DirectX;

Scene1::Scene1() :
	key_(nullptr)
{
}

Scene1::~Scene1()
{
	
}

void Scene1::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// ���f��
	mFloor_ = std::make_unique<Model>("floor");
	mSphere_ = std::make_unique<Model>("sphere");

	// �I�u�W�F�N�g
	oFloor_ = std::make_unique<Object3D>(mFloor_.get());
	oFloor_->SetPosition({ 0.0f, -1.0f, 0.0f });
	oFloor_->SetScale({ 10.0f, 10.0f, 10.0f });

	oSphere_.resize(2);

	oSphere_[0] = std::make_unique<Object3D>(mSphere_.get());
	oSphere_[0]->SetPosition({ 3.0f, 0.0f, 0.0f });

	oSphere_[1] = std::make_unique<Object3D>(mSphere_.get());
	oSphere_[1]->SetPosition({ -3.0f, 0.0f, 0.0f });

	// �f�B�]���u�𐶐�
	dissolve_ = std::make_unique<Dissolve3D>();
	dissolve_->Initialize();

	// �e�N�X�`���n���h��
	mainTexHandle_ = LoadTexture("Resources/mainTex.jpg");
	subTexHandle_ = LoadTexture("Resources/subTex.jpg");
	maskTexHandle_ = LoadTexture("Resources/maskTex.png");

	// ���C�g����
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();
	pointLight_ = std::make_unique<PointLight>();

	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	//pointLight_->SetLightPos({ -4.0f, 1.0f, 0.0f });

	lightGroup_->AddDirLight(dirLight_.get());
	//lightGroup_->AddPointLight(pointLight_.get());

	// �J������ݒ�
	Object3D::SetCamera(camera_.get());

	// ���C�g��ݒ�
	Object3D::SetLightGroup(lightGroup_.get());
}

void Scene1::Update()
{
	// �J�����ړ�
	{
		static float3 eye = { 0.0f, 10.0f, -30.0f };

		eye.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		eye.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;

		camera_->SetEye(eye);
	}

	// �I�u�W�F�N�g�̍X�V
	oFloor_->Update();
	oSphere_[0]->Update();
	oSphere_[1]->Update();

	static float t = 0.0f;

	if (key_->PushKey(DIK_UP)) t += 0.01f;
	if (key_->PushKey(DIK_DOWN)) t -= 0.01f;

	t = Util::Clamp(t, 1.01f, 0.0f);

	dissolve_->SetDissolveTime(t);

	dissolve_->Update();

	// �J�����̍X�V
	camera_->Update();

	// �Փ˔���
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	oSphere_[0]->Draw();
	oSphere_[1]->Draw();

	PipelineManager::GetInstance()->PreDraw("TextureBlend");

	oFloor_->TextureBlendDraw(mainTexHandle_, subTexHandle_, maskTexHandle_);

	dissolve_->Draw();
}