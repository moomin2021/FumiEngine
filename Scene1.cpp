#include "Scene1.h"
#include "Texture.h"

Scene1::Scene1() :
	key_(nullptr),
	lightGroup_(nullptr)
{
}

Scene1::~Scene1()
{
	delete lightGroup_;
	delete mCube_;
}

void Scene1::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// ���f��
	mCube_ = Model::CreateModel("cube");

	// �I�u�W�F�N�g
	oCube_.resize(3);

	oCube_[0] = std::make_unique<Object3D>(mCube_);
	oCube_[0]->SetPosition({ 3.0f, 1.0f, 0.0f });

	oCube_[1] = std::make_unique<Object3D>(mCube_);
	oCube_[1]->SetPosition({ -3.0f, 1.0f, 0.0f });

	oCube_[2] = std::make_unique<Object3D>(mCube_);
	oCube_[2]->SetPosition({ 0.0f, 0.0f, 0.0f });

	// �e�N�X�`���n���h��
	haeHandle_ = LoadTexture("Resources/hae.png");

	// �X�v���C�g
	sHae_ = std::make_unique<Sprite>();

	// ���C�g����
	lightGroup_ = LightGroup::Create();

	lightGroup_->SetDirLightActive(0, true);
	lightGroup_->SetDirLightActive(1, false);
	lightGroup_->SetDirLightActive(2, false);
	lightGroup_->SetPointLightActive(0, true);
	lightGroup_->SetPointLightActive(1, false);
	lightGroup_->SetPointLightActive(2, false);
	lightGroup_->SetSpotLightActive(0, false);
	lightGroup_->SetSpotLightActive(1, false);
	lightGroup_->SetSpotLightActive(2, false);
	lightGroup_->SetCircleShadowActive(0, false);

	// �J������ݒ�
	Object3D::SetCamera(camera_.get());

	// ���C�g��ݒ�
	Object3D::SetLightGroup(lightGroup_);
}

void Scene1::Update()
{
	if (key_->TriggerKey(DIK_0)) {
		int num = 0;
	}

	// �J�����ړ�
	{
		static float3 eye = { 0.0f, 10.0f, -30.0f };

		eye.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		eye.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;

		camera_->SetEye(eye);
	}

	static float3 pos = { 0.0f, 0.5f, 0.0f };

	pos.x += (key_->PushKey(DIK_RIGHT) - key_->PushKey(DIK_LEFT)) * 0.2f;
	pos.z += (key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN)) * 0.2f;

	lightGroup_->SetPointLightPos(0, XMFLOAT3({ pos.x, pos.y, pos.z }));

	static float rota = 0.0f;
	rota += 1.0f;

	oCube_[0]->SetRotation({ 0.0f, rota, 0.0f });
	oCube_[1]->SetRotation({0.0f, rota, 0.0f});

	// �J�����̍X�V
	camera_->Update();

	// ���C�g�̍X�V
	lightGroup_->Update();
}

void Scene1::Draw()
{
	Object3D::PreDraw();

	for (auto& object : oCube_) object->Draw();

	Sprite::PreDraw();

	sHae_->Draw();
}