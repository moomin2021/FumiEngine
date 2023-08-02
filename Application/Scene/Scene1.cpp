#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

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
	camera_->SetEye({ 0.0f, 1.0f, -5.0f });

	particle_ = std::make_unique<ParticleManager>();
	for (size_t i = 0; i < 100; i++) {
		float3 pos{};
		pos.x = Util::GetRandomFloat(-5.0f, 5.0f);
		pos.y = Util::GetRandomFloat(-5.0f, 5.0f);
		pos.z = Util::GetRandomFloat(-5.0f, 5.0f);

	particles_[0]->SetSpawnPos({ -2.0f, 1.0f, 0.0f });
	particles_[1]->SetSpawnPos({ 0.0f, 1.0f, 0.0f });
	particles_[2]->SetSpawnPos({ 2.0f, 1.0f, 0.0f });

		float3 acc{};
		acc.y = -Util::GetRandomFloat(-0.001f, 0.0f);

		particle_->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}

	// �p�[�e�B�N���ɃJ������ݒ�
	ParticleEmitter::SetCamera(camera_.get());

	// �p�[�e�B�N���̉摜�ǂݍ���
	particleHandle_ = LoadTexture("Resources/effect1.png");

	// ����
	soundKey_ = Sound::LoadWave("Resources/Sound/TestBGM.wav");
	Sound::SetVolume(soundKey_, 0.001f);
}

void Scene1::Update()
{
	// �J�����ړ�
	{
		static float3 eye = { 0.0f, 3.0f, -20.0f };

		eye.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.5f;
		eye.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.5f;

		camera_->SetEye(eye);
	}
 
	// �J�����̍X�V
	camera_->Update();

	// �p�[�e�B�N���X�V
	particles_[0]->Update(BILLBOARD::ALL);
	particles_[1]->Update(BILLBOARD::ALL);
	particles_[2]->Update(BILLBOARD::ALL);

	// �Փ˔���
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene1::Draw()
{
	PipelineManager::GetInstance()->PreDraw("Particle", D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// �p�[�e�B�N���`��
	particle_->Draw(particleHandle_);
}