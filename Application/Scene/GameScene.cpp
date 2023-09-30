#include "GameScene.h"
#include "CollisionManager.h"

#include "PipelineManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize()
{
#pragma region ���C�g�O���[�v
	lightGroup_ = std::make_unique<LightGroup>();
	Object3D::SetLightGroup(lightGroup_.get());

	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

#pragma region �X�e�[�W�N���X
	stage_ = std::make_unique<Stage>();
	stage_->Initialize();
	stage_->Load("Resources/StageJson/stage1.json");
#pragma endregion

#pragma region �v���C���[
	player_ = std::make_unique<Player>();
	player_->Initialize();
#pragma endregion
}

void GameScene::Update()
{
	// �v���C���[
	player_->Update();

	// �Փˎ�����
	OnCollision();

	// �s��X�V����
	MatUpdate();
}

void GameScene::Draw()
{
	// �X�e�[�W�N���X
	stage_->Draw();

	// �v���C���[
	player_->Draw3D();
}

void GameScene::OnCollision()
{
	// �Փ˔�����Ƃ�
	CollisionManager::GetInstance()->CheckAllCollision();

	// �v���C���[
	player_->OnCollision();
}

void GameScene::MatUpdate()
{
	// �X�e�[�W�N���X
	stage_->MatUpdate();

	// �v���C���[
	player_->MatUpdate();
}