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

	// �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 10.0f, -30.0f });

	// ���f��
	mFloor_ = std::make_unique<Model>("floor");
	mCube_ = std::make_unique<Model>("cube");
	mSphere_ = std::make_unique<Model>("sphere");

	// �I�u�W�F�N�g
	oFloor_ = std::make_unique<Object3D>(mFloor_.get());
	oFloor_->SetScale({ 10.0f, 10.0f, 10.0f });

	oSphere_ = std::make_unique<Object3D>(mSphere_.get());
	oSphere_->SetPosition({ 0.0f, 1.0f, -3.0f });
	oSphere_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	oCube_.resize(3);

	oCube_[0] = std::make_unique<Object3D>(mCube_.get());
	oCube_[0]->SetPosition({ 3.0f, 1.0f, 0.0f });

	oCube_[1] = std::make_unique<Object3D>(mCube_.get());
	oCube_[1]->SetPosition({ -3.0f, 1.0f, 0.0f });

	oCube_[2] = std::make_unique<Object3D>(mCube_.get());
	oCube_[2]->SetPosition({ 0.0f, 1.0f, 0.0f });

	// �e�N�X�`���n���h��
	haeHandle_ = LoadTexture("Resources/hae.png");

	// ���C�g����
	lightGroup_ = std::make_unique<LightGroup>();
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	//pointLight_->SetLightPos({ -4.0f, 1.0f, 0.0f });

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

	// �J�����ړ�
	{
		static float3 eye = { 0.0f, 10.0f, -30.0f };
		// �X�e�[�W�X�V
		loadStage_->Update();

		// �v���C���[�X�V
		player_->Update();

		Collision();
	}
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �v���C���[�̕`�揈��
	player_->Draw();

	// �X�e�[�W�`��
	loadStage_->Draw();

	oFloor_->Draw();
	oSphere_->Draw();
	for (auto& object : oCube_) object->Draw();

	PipelineManager::PreDraw("Sprite");

	player_->Draw2D();
}

void Scene1::Collision()
{

	//sHae_->Draw();
	// �e�ƓG�̏Փ˔���
	//for (size_t i = 0; i < loadStage_->enemys_.size(); i++) {
	//	if (loadStage_->enemys_[i]->GetIsAlive()) {
	//		for (size_t j = 0; j < player_->bullets_.size(); j++) {
	//			if (SphereCollision(loadStage_->enemys_[i]->GetSphereCol(), player_->bullets_[j]->GetSphereCol())) {
	//			loadStage_->enemys_[i]->ReduceHP(1);
	//				player_->bullets_.erase(player_->bullets_.begin() + j);
	//			}
	//		}
	//	}
	//}
}