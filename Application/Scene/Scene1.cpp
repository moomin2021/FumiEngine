#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "Collision.h"

#include <DirectXMath.h>

using namespace DirectX;

bool SphereCollision(const SphereCol& s0, const SphereCol& s1) {
	float a = s0.pos.x - s1.pos.x;
	float b = s0.pos.y - s1.pos.y;
	float c = s0.pos.z - s1.pos.z;
	float d = sqrtf(a * a + b * b + c * c);
	if (d <= s0.radius + s1.radius) return true;
	return false;
}

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

	Collision();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �v���C���[�̕`�揈��
	player_->Draw();

	// �X�e�[�W�`��
	loadStage_->Draw();

	PipelineManager::PreDraw("Sprite");

	player_->Draw2D();
}

void Scene1::Collision()
{
	// �e�ƓG�̏Փ˔���
	for (size_t i = 0; i < loadStage_->enemys_.size(); i++) {
		if (loadStage_->enemys_[i]->GetIsAlive()) {
			for (size_t j = 0; j < player_->bullets_.size(); j++) {
				if (SphereCollision(loadStage_->enemys_[i]->GetSphereCol(), player_->bullets_[j]->GetSphereCol())) {
				loadStage_->enemys_[i]->ReduceHP(1);
					player_->bullets_.erase(player_->bullets_.begin() + j);
				}
			}
		}
	}
}