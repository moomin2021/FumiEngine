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
#pragma endregion

#pragma region �v���C���[
	player_ = std::make_unique<Player>();
	player_->Initialize();
#pragma endregion

#pragma region �G�l�~�[�}�l�[�W���[
	// �G�l�~�[�}�l�[�W���[����
	enemyMgr_ = std::make_unique<EnemyManager>();
	enemyMgr_->Initialize();
	enemyMgr_->SetPlayer(player_.get());
	stage_->SetEnemyManager(enemyMgr_.get());
	player_->SetEnemyManager(enemyMgr_.get());
#pragma endregion

#pragma region ���f��
	mCube_ = std::make_unique<Model>("cube");
#pragma endregion

#pragma region 3���������I�u�W�F�N�g
	oAxis_.resize(3);
	oAxis_[0] = std::make_unique<Object3D>(mCube_.get());
	oAxis_[1] = std::make_unique<Object3D>(mCube_.get());
	oAxis_[2] = std::make_unique<Object3D>(mCube_.get());

	oAxis_[0]->SetPosition({ 5.0f, 0.0f, 0.0f });
	oAxis_[1]->SetPosition({ 0.0f, 5.0f, 0.0f });
	oAxis_[2]->SetPosition({ 0.0f, 0.0f, 5.0f });

	oAxis_[0]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	oAxis_[1]->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	oAxis_[2]->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
#pragma endregion

	// �X�e�[�W�ǂݍ���
	stage_->Load("Resources/StageJson/stage1.json");
}

void GameScene::Update()
{
	// �v���C���[
	player_->Update();

	// �G�l�~�[�}�l�[�W���[
	enemyMgr_->Update();

	// �Փˎ�����
	OnCollision();

	// �s��X�V����
	MatUpdate();

	enemyMgr_->CheckSceneChange();
}

void GameScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �X�e�[�W�N���X
	stage_->Draw();

	// �v���C���[
	player_->Draw3D();

	// �G�l�~�[�}�l�[�W���[
	//enemyMgr_->Draw();

	// 3���������I�u�W�F�N�g
	for (auto& it : oAxis_) {
		it->Draw();
	}

	PipelineManager::PreDraw("Sprite");

	// �v���C���[
	player_->DrawFront2D();
}

void GameScene::OnCollision()
{
	// �Փ˔�����Ƃ�
	CollisionManager::GetInstance()->CheckAllCollision();

	// �v���C���[
	player_->OnCollision();

	// �G�l�~�[�}�l�[�W���[
	enemyMgr_->OnCollision();
}

void GameScene::MatUpdate()
{
	// �v���C���[
	player_->MatUpdate();

	// �G�l�~�[�}�l�[�W���[
	enemyMgr_->MatUpdate();

	// �X�e�[�W�N���X
	stage_->MatUpdate();

	// 3���������I�u�W�F�N�g
	for (auto& it : oAxis_) {
		it->MatUpdate();
	}
}