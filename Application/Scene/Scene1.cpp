#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "CollisionManager.h"

#include <DirectXMath.h>

using namespace DirectX;

Scene1::Scene1() :
	lightGroup_(nullptr),
	dirLight_(nullptr),

	// �X�e�[�W�ǂݍ��ݗp
	loadStage_(nullptr),

	// �v���C���[
	player_(nullptr)
{
}

Scene1::~Scene1()
{
	
}

void Scene1::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �G�Ǘ��N���X�C���X�^���X�擾
	enemyManager_ = EnemyManager::GetInstance();

	// �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// ���C�g����
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });

	lightGroup_->AddDirLight(dirLight_.get());

	// ���C�g��K�p
	Object3D::SetLightGroup(lightGroup_.get());

	// �X�e�[�W�ǂݍ��ݗp�N���X����
	loadStage_ = std::make_unique<LoadStage>();

	// �v���C���[����
	player_ = std::make_unique<Player>();
}

void Scene1::Update()
{
	// �Փ˔���
	CollisionManager::GetInstance()->CheckAllCollision();

	// �X�e�[�W�X�V
	loadStage_->Update();

	// �v���C���[�X�V
	player_->Update();

	// �G�X�V
	enemyManager_->Update();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �v���C���[�̕`�揈��
	player_->Draw();

	// �X�e�[�W�`��
	loadStage_->Draw();

	// �G�`��
	enemyManager_->Draw();

	PipelineManager::PreDraw("Sprite");

	player_->Draw2D();
}