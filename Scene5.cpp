#include "Scene5.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>

Scene5::Scene5() :
	key_(nullptr),
	camera_(nullptr),
	lightGroup_(nullptr),
	sphereM_(nullptr),
	rayM_(nullptr),
	object_{},
	rayC_{},
	sphereC_{},
	count_(0)
{
}

Scene5::~Scene5()
{
	delete camera_;
	delete lightGroup_;
	delete sphereM_;
	delete rayM_;
	delete object_[0];
	delete object_[1];
	delete object_[2];
}

void Scene5::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -10.0f };

	// ���f��
	sphereM_ = Model::CreateModel("sphere");
	rayM_ = Model::CreateModel("ray");

	// �I�u�W�F�N�g
	object_[0] = Object3D::CreateObject3D(sphereM_);
	object_[0]->SetPos({ 0.0f, 5.0f, 0.0f });

	object_[1] = Object3D::CreateObject3D(rayM_);
	object_[1]->SetPos({ 0.0f, 5.0f, 0.0f });
	object_[1]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	object_[2] = Object3D::CreateObject3D(sphereM_);
	object_[2]->SetPos({ 0.0f, 0.0f, 0.0f });

	// ���C�g����
	lightGroup_ = LightGroup::Create();

	lightGroup_->SetDirLightActive(0, true);
	lightGroup_->SetDirLightActive(1, false);
	lightGroup_->SetDirLightActive(2, false);

	// �J������ݒ�
	Object3D::SetCamera(camera_);

	// ���C�g��ݒ�
	Object3D::SetLightGroup(lightGroup_);

	// ���C�̏����l��ݒ�
	rayC_.start = XMVectorSet(0.0f, 5.0f, 0.0f, 1.0f);// -> ���_����
	rayC_.dir = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);// -> ������

	// ���̏����l��ݒ�
	sphereC_.center = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);// -> ���S�_���W
	sphereC_.radius = 1.0f;// -> ���a
}

void Scene5::Update()
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
		Float3 defaPos = { 0.0f, 5.0f, 0.0f };
		defaPos.x += sinf((float)count_ * 0.05f) * 3.0f;
		defaPos.z += sinf((float)count_ * 0.05f) * 3.0f;

		rayC_.start.m128_f32[0] = defaPos.x;
		rayC_.start.m128_f32[2] = defaPos.z;

		object_[0]->SetPos(defaPos);
		object_[1]->SetPos(defaPos);

		float distance = 0.0f;

		if (Collision::CheckRay2Sphere(rayC_, sphereC_, &distance)) {
			object_[0]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		}
		else {
			object_[0]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}

		object_[1]->SetScale({ 1.0f, distance / 2.0f, 1.0f });
	}

	// �J�����̍X�V
	camera_->Update();

	// ���C�g�̍X�V
	lightGroup_->Update();
}

void Scene5::Draw()
{
	Object3D::PreDraw();

	for (auto& object : object_) object->Draw();
}