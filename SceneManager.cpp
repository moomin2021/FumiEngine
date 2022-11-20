#include "SceneManager.h"
#include "Scene1.h"
#include "Scene2.h"

// ���݂̃V�[������ɂ���
BaseScene* SceneManager::myScene_ = nullptr;

SceneManager::SceneManager() {
	// �C���X�^���X�擾
	key_ = Key::GetInstance();
}

SceneManager::~SceneManager() {
	// ���݂̃V�[������ł͂Ȃ�������
	if (myScene_ != nullptr) {
		delete myScene_;// -> ���݂̃V�[�����������������
	}
}

void SceneManager::ChangeScene(SCENE scene) {
	// ���݂̃V�[������ł͂Ȃ�������
	if (myScene_ != nullptr) {
		delete myScene_;// -> ���݂̃V�[�����������������
	}

	switch (scene)
	{
	// �V�[��1�ɐݒ�
	case SCENE::SCENE1:
		myScene_ = new Scene1();
		myScene_->Initialize();
		break;
	case SCENE::SCENE2:
		myScene_ = new Scene2();
		myScene_->Initialize();
	default:
		break;
	}
}

void SceneManager::Update() {
	// ���݂̃V�[���̍X�V����
	myScene_->Update();

	if (key_->TriggerKey(DIK_1)) {
		ChangeScene(SCENE1);
	}

	else if (key_->TriggerKey(DIK_2)) {
		ChangeScene(SCENE2);
	}
}

void SceneManager::Draw() {
	// ���݂̃V�[���̕`�揈��
	myScene_->Draw();
}