#include "SceneManager.h"
#include "Scene1.h"

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
	default:
		break;
	}
}

void SceneManager::Update() {
	// ���݂̃V�[���̍X�V����
	myScene_->Update();
}

void SceneManager::Draw() {
	// ���݂̃V�[���̕`�揈��
	myScene_->Draw();
}