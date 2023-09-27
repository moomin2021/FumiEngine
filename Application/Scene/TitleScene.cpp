#include "TitleScene.h"

#include "PipelineManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize()
{
#pragma region �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 0.0f, -10.0f });
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region ���C�g�O���[�v
	lightGroup_ = std::make_unique<LightGroup>();
	Object3D::SetLightGroup(lightGroup_.get());

	dirLight_ = std::make_unique<DirectionalLight>();
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region ���f��
	model_ = std::make_unique<Model>("cube");
#pragma endregion

#pragma region �I�u�W�F�N�g3D
	object_ = std::make_unique<Object3D>(model_.get());
#pragma endregion
}

void TitleScene::Update()
{
	// �Փˎ�����
	OnCollision();

	// �s��X�V����
	MatUpdate();
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �I�u�W�F�N�g
	object_->Draw();
}

void TitleScene::OnCollision()
{
}

void TitleScene::MatUpdate()
{
	// �J����
	camera_->Update();

	// �I�u�W�F�N�g3D
	object_->MatUpdate();
}