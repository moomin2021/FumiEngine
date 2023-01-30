#include "Scene1.h"

Scene1::Scene1() :
	key_(nullptr),
	camera_(nullptr),
	sphereM_(nullptr),
	sphereObj_{}
{
}

Scene1::~Scene1()
{
	delete camera_;
	delete sphereM_;
	delete sphereObj_[0];
	delete sphereObj_[1];
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

	// �I�u�W�F�N�g
	sphereObj_[0] = Object3D::CreateObject3D();
	sphereObj_[0]->SetPos({ 3.0f, 0.0f, 0.0f });
	sphereObj_[0]->SetModel(sphereM_);

	sphereObj_[1] = Object3D::CreateObject3D();
	sphereObj_[1]->SetPos({ -3.0f, 0.0f, 0.0f });
	sphereObj_[1]->SetModel(sphereM_);

	// �J������ݒ�
	Object3D::SetCamera(camera_);
}

void Scene1::Update()
{
	if (key_->TriggerKey(DIK_0)) {
		int num = 0;
	}

	// �J�����ړ�
	{
		camera_->eye_.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		camera_->eye_.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;
	}

	static float rota = 0.0f;
	rota += 1.0f;

	sphereObj_[0]->SetRot({ 0.0f, rota, 0.0f });
	sphereObj_[1]->SetRot({0.0f, rota, 0.0f});

	// �J�����̍X�V
	camera_->Update();
}

void Scene1::Draw()
{
	Object3D::PreDraw();

	sphereObj_[0]->Draw();
	sphereObj_[1]->Draw();
}