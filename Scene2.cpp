#include "Scene2.h"
#include "FumiEngine.h"
#include <time.h>

Scene2::Scene2() :
#pragma region ���������X�g
	camera_(nullptr),// -> �J����
	billBoard_{}// -> �r���{�[�h
#pragma endregion
{

}

Scene2::~Scene2() {
	delete camera_;
	for (size_t i = 0; i < 10; i++) {
		delete billBoard_[i];
	}
}

void Scene2::Initialize() {
	srand(time(nullptr));

	// �L�[�{�[�h�N���X�C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J��������
	camera_ = new Camera();
	camera_->eye_ = { 0.0f, 0.0f, -10.0f };

	// �r���{�[�h����
	for (size_t i = 0; i < 10; i++) {
		billBoard_[i] = new BillBoard();
		billBoard_[i]->position_.x = (rand() % 6 - 2) * 0.5f;
		billBoard_[i]->position_.z = (rand() % 6 - 2) * 0.5f;
		billBoard_[i]->position_.z = (rand() % 6 - 2) * 0.5f;
	}

	// �摜�n���h���擾
	valoHandle_ = LoadTexture(L"Resources/kusa.png");
}

void Scene2::Update() {
	if (key_->TriggerKey(DIK_R)) {
		camera_->eye_ = { 0.0f, 0.0f, -10.0f };
	}

	// �J�����ړ�
	camera_->eye_.x += key_->PushKey(DIK_D) - key_->PushKey(DIK_A);
	camera_->eye_.y += key_->PushKey(DIK_W) - key_->PushKey(DIK_S);
	camera_->eye_.z += key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN);

	// �J�����̍X�V����
	camera_->Update();

	// �r���{�[�h�X�V����
	for (size_t i = 0; i < 10; i++) {
		billBoard_[i]->Update(camera_, BillBoardType::BILLBOARDY);
	}
}

void Scene2::Draw() {
	// �I�u�W�F�N�g�`��O����
	BillBoard::PreDraw();

	// �r���{�[�h�`�揈��
	for (size_t i = 0; i < 10; i++) {
		billBoard_[i]->Draw(valoHandle_);
	}
}