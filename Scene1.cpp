#include "Scene1.h"

Scene1::Scene1() :
	key_(nullptr),
	camera_(nullptr),
	lightGroup_(nullptr),
	sphereM_(nullptr),
	object_{}
{
}

Scene1::~Scene1()
{
	delete camera_;
	delete lightGroup_;
	delete sphereM_;
	delete cubeM_;
	delete floorM_;
	delete object_[0];
	delete object_[1];
	delete object_[2];
}

void Scene1::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// ���f��
	sphereM_ = Model::CreateModel("sphere");
	cubeM_ = Model::CreateModel("cube");
	floorM_ = Model::CreateModel("floor");

	// �I�u�W�F�N�g
	object_[0] = Object3D::CreateObject3D(sphereM_);
	object_[0]->SetPos({ 3.0f, 1.0f, 0.0f });

	object_[1] = Object3D::CreateObject3D(sphereM_);
	object_[1]->SetPos({ -3.0f, 1.0f, 0.0f });

	object_[2] = Object3D::CreateObject3D(floorM_);
	object_[2]->SetPos({ 0.0f, 0.0f, 0.0f });
	object_[2]->SetScale({ 10.0f, 10.0f, 10.0f });

	// ���C�g����
	lightGroup_ = LightGroup::Create();

	lightGroup_->SetDirLightActive(0, false);
	lightGroup_->SetDirLightActive(1, false);
	lightGroup_->SetDirLightActive(2, false);
	lightGroup_->SetPointLightActive(0, true);
	lightGroup_->SetPointLightActive(1, true);
	lightGroup_->SetPointLightActive(2, false);
	lightGroup_->SetSpotLightActive(0, true);
	lightGroup_->SetSpotLightActive(1, false);
	lightGroup_->SetSpotLightActive(2, false);
	lightGroup_->SetCircleShadowActive(0, true);

	lightGroup_->SetCircleShadowCasterPos(0, XMFLOAT3({ -3.0f, 1.0f, 0.0f }));
	lightGroup_->SetCircleShadowDir(0, XMVECTOR({ 0.0f, -1.0f, 0.0f }));
	lightGroup_->SetCircleShadowAtten(0, XMFLOAT3({ 0.5f, 0.6f, 0.0f }));
	lightGroup_->SetCircleShadowFactorAngle(0, XMFLOAT2({ 0.0f, 0.5f }));

	lightGroup_->SetSpotLightDir(0, XMVECTOR({ 0.0f, -1.0f, 0.0f }));
	lightGroup_->SetSpotLightAtten(0, XMFLOAT3({ 0.0f, 0.0f, 0.0f }));
	lightGroup_->SetSpotLightFactorAngle(0, XMFLOAT2({ 20.0f, 30.0f }));

	// �J������ݒ�
	Object3D::SetCamera(camera_);

	// ���C�g��ݒ�
	Object3D::SetLightGroup(lightGroup_);
}

void Scene1::Update()
{
	if (key_->TriggerKey(DIK_0)) {
		int num = 0;
	}

	static int count = 0;
	count++;

	// �J�����ړ�
	{
		camera_->eye_.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		camera_->eye_.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;
	}

	if (key_->TriggerKey(DIK_E)) {
		object_[0]->SetModel(cubeM_);
	}

	if (key_->TriggerKey(DIK_Q)) {
		object_[0]->SetModel(sphereM_);
	}

	static Float3 pos0 = { 0.0f, 0.5f, 0.0f };
	static Float3 pos1 = { 0.0f, 0.5f, 0.0f };
	static Float3 pos2 = { 0.0f, 5.0f, -2.0f };

	pos0.x += (key_->PushKey(DIK_RIGHT) - key_->PushKey(DIK_LEFT)) * 0.2f;
	pos0.z += (key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN)) * 0.2f;

	pos1.x = sinf(count * 0.1f) * 3.0f;
	pos2.x = sinf(count * 0.1f) * 4.0f;

	lightGroup_->SetPointLightPos(0, XMFLOAT3({ pos0.x, pos0.y, pos0.z }));
	lightGroup_->SetPointLightPos(1, XMFLOAT3({ pos1.x, pos1.y, pos1.z }));
	lightGroup_->SetSpotLightPos(0, XMFLOAT3({ pos2.x, pos2.y, pos2.z }));

	static float rota = 0.0f;
	rota += 1.0f;

	object_[0]->SetRot({ 0.0f, rota, 0.0f });
	object_[1]->SetRot({0.0f, rota, 0.0f});

	// �J�����̍X�V
	camera_->Update();

	// ���C�g�̍X�V
	lightGroup_->Update();
}

void Scene1::Draw()
{
	Object3D::PreDraw();

	for (auto& object : object_) object->Draw();
}