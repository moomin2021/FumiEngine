#include "Scene2.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>

Scene2::Scene2() :
	key_(nullptr),
	camera_(nullptr),
	lightGroup_(nullptr),
	sphereM_(nullptr),
	triangleM_(nullptr),
	object_{},
	sphereC_{},
	triangleC_{},
	count_(0)
{
}

Scene2::~Scene2()
{
	delete camera_;
	delete lightGroup_;
	delete sphereM_;
	delete triangleM_;
	delete object_[0];
	delete object_[1];
}

void Scene2::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 20.0f, -30.0f };

	// ���f��
	sphereM_ = Model::CreateModel("sphere");
	triangleM_ = Model::CreateModel("triangle");

	// �I�u�W�F�N�g
	object_[0] = Object3D::CreateObject3D(sphereM_);
	object_[0]->SetPos({ 0.0f, 0.0f, 0.0f });

	object_[1] = Object3D::CreateObject3D(triangleM_);
	object_[1]->SetPos({ 0.0f, 0.0f, 0.0f });
	object_[1]->SetRot({ 0.0f, 90.0f, 0.0f });
	object_[1]->SetScale({ 5.0f, 1.0f, 5.0f });

	// ���C�g����
	lightGroup_ = LightGroup::Create();

	lightGroup_->SetDirLightActive(0, true);
	lightGroup_->SetDirLightActive(1, false);
	lightGroup_->SetDirLightActive(2, false);

	// �J������ݒ�
	Object3D::SetCamera(camera_);

	// ���C�g��ݒ�
	Object3D::SetLightGroup(lightGroup_);

	// �e�̏����l��ݒ�
	sphereC_.center = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);// -> ���S�_���W
	sphereC_.radius = 1.0f;// -> ���a

	// �O�p�`�̏����l��ݒ�
	triangleC_.p0 = XMVectorSet(-5.0f, 0.0f, -5.0f, 1.0f);// -> ����O
	triangleC_.p1 = XMVectorSet(-5.0f, 0.0f, 5.0f, 1.0f);// -> ����
	triangleC_.p2 = XMVectorSet(5.0f, 0.0f, -5.0f, 1.0f);// -> �E��O
	triangleC_.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);// -> �����
}

void Scene2::Update()
{
	if (key_->TriggerKey(DIK_0)) {
		int num = 0;
	}

	// �J�E���g�i�s
	count_++;

	// �J�����ړ�
	{
		camera_->eye_.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		camera_->eye_.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;
	}

	// ���ړ�
	{
		Float3 defaPos = { 0.0f, 0.0f, 0.0f };
		defaPos.x += sinf((float)count_ * 0.05f) * 7.0f;
		defaPos.z += sinf((float)count_ * 0.05f) * 7.0f;

		sphereC_.center.m128_f32[0] = defaPos.x;
		sphereC_.center.m128_f32[2] = defaPos.z;

		object_[0]->SetPos(defaPos);

		if (Collision::CheckSphere2Triangle(sphereC_, triangleC_)) {
			object_[0]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		}
		else {
			object_[0]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}

	// �J�����̍X�V
	camera_->Update();

	// ���C�g�̍X�V
	lightGroup_->Update();
}

void Scene2::Draw()
{
	Object3D::PreDraw();

	for (auto& object : object_) object->Draw();
}