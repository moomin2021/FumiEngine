#include "GameOverScene.h"
#include "PipelineManager.h"
#include "SceneManager.h"
#include "Texture.h"

GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {}

void GameOverScene::Initialize()
{
	// �C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J�����ݒ�
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 0.0f, -5.0f });

	// �X�v���C�g����
	sGameOver_ = std::make_unique<Sprite>();
	sGameOver_->SetSize({ 1920.0f, 1080.0f });

	// �摜�ǂݍ���
	gameOverHandle_ = LoadTexture("Resources/GameOver.png");
}

void GameOverScene::Update()
{
	// �J�����X�V
	camera_->Update();

	if (key_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene(SCENE::TITLE);
	}
}

void GameOverScene::Draw()
{
	PipelineManager::PreDraw("Sprite");

	// �X�v���C�g�`��
	sGameOver_->Draw(gameOverHandle_);
}