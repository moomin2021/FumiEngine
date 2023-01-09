#include "TitleScene.h"
#include "FumiEngine.h"

TitleScene::TitleScene() :
	key_(nullptr),
	player_(nullptr),
	object_(nullptr),
	camera_(nullptr),
	sprite_(nullptr)
{
}

TitleScene::~TitleScene()
{
	delete player_;
	delete object_;
	delete camera_;
	delete sprite_;
}

void TitleScene::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 10.0f, -30.0f };

	// �v���C���[���f��
	player_ = player_->CreateModel("player_rest");

	// �I�u�W�F�N�g
	object_ = Object3D::CreateObject3D();
	object_->SetCamera(camera_);
	object_->SetModel(player_);
}

void TitleScene::Update()
{
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.z += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);

	// �J�����̍X�V
	camera_->Update();

	// �I�u�W�F�N�g�X�V
	object_->rotation_.y += 0.1f;
}

void TitleScene::Draw()
{
	// ���f���`��O����
	Object3D::PreDraw();

	// �v���C���[���f���`��
	object_->Draw();
}
