#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"

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
	// ���C�g�������ݒ�
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
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
	// �X�e�[�W�X�V
	loadStage_->Update();

	// �v���C���[�X�V
	player_->Update();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �X�e�[�W�`��
	loadStage_->Draw();
}