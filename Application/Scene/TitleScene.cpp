#include "TitleScene.h"
#include "Texture.h"
#include "PipelineManager.h"
#include "SceneManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
#pragma region �C���X�^���X�擾
	key_ = Key::GetInstance();// �}�E�X
#pragma endregion

#pragma region �X�v���C�g
	sTitle_ = std::make_unique<Sprite>();
	sTitle_->SetSize(Vector2{1920.0f, 1080.0f});
#pragma endregion

#pragma region �e�N�X�`���n���h��
	hTitle_ = LoadTexture("Resources/Title.png");
#pragma endregion
}

void TitleScene::Update()
{
	if (key_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->SceneTransition(GAME);
	}
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Sprite");

	// �^�C�g����`��
	sTitle_->Draw(hTitle_);
}

void TitleScene::OnCollision()
{
}

void TitleScene::ObjUpdate()
{
}
