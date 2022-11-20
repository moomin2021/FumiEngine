#include "Scene1.h"
#include "FumiEngine.h"

Scene1::Scene1() :
#pragma region ���������X�g
	camera_(nullptr),// -> �J����
	object_(nullptr)// -> �I�u�W�F�N�g
#pragma endregion
{

}

Scene1::~Scene1() {
	delete camera_;
	delete object_;
}

void Scene1::Initialize() {
	// �L�[�{�[�h�N���X�C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J��������
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 0.0f, -10.0f };

	// �I�u�W�F�N�g����
	object_ = new Object3D();
	object_->CreateCube();
	object_->position_ = { -3.0f, 0.0f, 0.0f };

	// �摜�n���h���擾
	valoHandle_ = LoadTexture(L"Resources/valo.jpg");
}

void Scene1::Update() {
	// �J�����ړ�
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.y += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);
	camera_->eye_.z += key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN);

	// �J�����̍X�V����
	camera_->Update();

	// �I�u�W�F�N�g�X�V����
	object_->Update(camera_);
}

void Scene1::Draw() {
	// �I�u�W�F�N�g�`��O����
	Object3D::PreDraw();

	// �I�u�W�F�N�g�`�揈��
	object_->Draw(valoHandle_);
}