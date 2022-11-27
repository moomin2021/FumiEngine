#include "GameScene.h"

// �R���X�g���N�^
GameScene::GameScene() :
	sprite_(nullptr)// -> �X�v���C�g
{

}

// �f�X�g���N�^
GameScene::~GameScene() {
	delete sprite_;// -> �X�v���C�g
}

// ����������
void GameScene::Initialize() {
	// �X�v���C�g
	sprite_ = new Sprite();
}

// �X�V����
void GameScene::Update() {
	sprite_->Update();
}

// �`�揈��
void GameScene::Draw() {
	Sprite::PreDraw();
	sprite_->Draw();
}