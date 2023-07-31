#include "TitleScene.h"
#include "PipelineManager.h"
#include "SceneManager.h"
#include "Texture.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize()
{
	// �C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J�����ݒ�
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 0.0f, -5.0f });

	// �X�v���C�g����
	sTitle_ = std::make_unique<Sprite>();
	sTitle_->SetSize({ 1920.0f, 1080.0f });

	// �摜�ǂݍ���
	titleHandle_ = LoadTexture("Resources/title.png");
}

void TitleScene::Update()
{
	// �J�����X�V
	camera_->Update();

	if (key_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene(SCENE::SCENE1);
	}
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Sprite");

	// �X�v���C�g�`��
	sTitle_->Draw(titleHandle_);
}