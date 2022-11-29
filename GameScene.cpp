#include "GameScene.h"

// �R���X�g���N�^
GameScene::GameScene() :
	key_(nullptr),// -> �L�[����
	sprite_(nullptr)// -> �X�v���C�g
{

}

// �f�X�g���N�^
GameScene::~GameScene() {
	delete sprite_;// -> �X�v���C�g
}

// ����������
void GameScene::Initialize() {
	// �C���X�^���X�擾
	key_ = Key::GetInstance();

	// �X�v���C�g
	sprite_ = new Sprite();

	// �`��t���O
	isDraw_ = false;
}

// �X�V����
void GameScene::Update() {
	// [SPACE]�L�[����������`��t���O��؂�ւ���
	if (key_->TriggerKey(DIK_SPACE)) isDraw_ = !isDraw_;

	// �X�v���C�g�ړ�����
	sprite_->position.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	sprite_->position.y += key_->PushKey(DIK_S) - key_->PushKey(DIK_W);

	// �X�v���C�g�X�V����
	sprite_->Update();
}

// �`�揈��
void GameScene::Draw() {
	Sprite::PreDraw();
	// �`��t���O��[ON]�Ȃ�`��
	if (isDraw_) sprite_->Draw();
}