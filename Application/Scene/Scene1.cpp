#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "Sound.h"
#include "CollisionManager.h"

Scene1::Scene1() :
	key_(nullptr)
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
	camera_->SetEye({ 0.0f, 1.0f, -10.0f });

	// �J�����Z�b�g
	Object3D::SetCamera(camera_.get());
	ParticleEmitter::SetCamera(camera_.get());

	// ���C�g�O���[�v����
	lightGroup_ = std::make_unique<LightGroup>();

	// ���C�g�O���[�v�Z�b�g
	Object3D::SetLightGroup(lightGroup_.get());

	// ���s��������
	dirLight_ = std::make_unique<DirectionalLight>();

	// ���s�����Z�b�g
	lightGroup_->AddDirLight(dirLight_.get());

	// ���f������
	model_ = std::make_unique<Model>("sphere");

	// �I�u�W�F�N�g����
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition({ -1.0f, 0.0f, 0.0f });

	// �p�[�e�B�N���G�~�b�^�[����
	particleEmitter_ = std::make_unique<ParticleEmitter>();
	particleEmitter_->SetSpawnPos({ 1.0f, 0.0f, 0.0f });

	// �p�[�e�B�N���p�摜�ǂݍ���
	particlehandle_ = LoadTexture("Resources/effect1.png");

	// �T�E���h�ǂݍ��݁��Đ�
	bgmKey_ = Sound::LoadWave("Resources/Sound/a.wav");
	Sound::SetVolume(bgmKey_, 0.001f);
	Sound::Play(bgmKey_);
}

void Scene1::Update()
{
	// �p�[�e�B�N������
	float3 pos{};// ���W
	pos.x = Util::GetRandomFloat(-0.1f, 0.1f);
	pos.y = Util::GetRandomFloat(-0.1f, 0.1f);
	pos.z = Util::GetRandomFloat(-0.1f, 0.1f);

	// ����
	float3 vel{};
	vel.x = Util::GetRandomFloat(-0.1f, 0.1f);
	vel.y = Util::GetRandomFloat(-0.1f, 0.1f);
	vel.z = Util::GetRandomFloat(-0.1f, 0.1f);

	// �����x
	float3 acc{};
	acc.x = Util::GetRandomFloat(-0.001f, 0.0f);
	acc.y = Util::GetRandomFloat(-0.001f, 0.0f);
	acc.z = Util::GetRandomFloat(-0.001f, 0.0f);

	particleEmitter_->Add(60, pos, vel, acc, 0.5f, 0.0f);

	// �p�[�e�B�N���X�V
	particleEmitter_->Update(BILLBOARD::ALL);
 
	// �I�u�W�F�N�g�X�V
	object_->Update();

	// �J�����̍X�V
	camera_->Update();

	// �Փ˔���
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Particle", D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	// �p�[�e�B�N���`��
	particleEmitter_->Draw(particlehandle_);

	PipelineManager::PreDraw("Object3D");

	// �I�u�W�F�N�g�`��
	object_->Draw();
}