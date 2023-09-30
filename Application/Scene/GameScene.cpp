#include "GameScene.h"

#include "PipelineManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize()
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

#pragma region �X�e�[�W�N���X
	stage_ = std::make_unique<Stage>();
	stage_->Initialize();
	stage_->Load("Resources/StageJson/stage1.json");
#pragma endregion
}

void GameScene::Update()
{
	// �Փˎ�����
	OnCollision();

	// �s��X�V����
	MatUpdate();
}

void GameScene::Draw()
{
	// �X�e�[�W�N���X
	stage_->Draw();
}

void GameScene::OnCollision()
{
}

void GameScene::MatUpdate()
{
	// �J����
	camera_->Update();

	// �X�e�[�W�N���X
	stage_->MatUpdate();
}