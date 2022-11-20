#include "SceneManager.h"

// ���݂̃V�[������ɂ���
BaseScene* SceneManager::myScene = nullptr;

void SceneManager::ChangeScene(SCENE scene) {
	// ���݂̃V�[������ł͂Ȃ�������
	if (myScene != nullptr) {
		delete myScene;// -> ���݂̃V�[�����������������
	}

	switch (scene)
	{
	case SCENE::SCENE1:
		break;
	default:
		break;
	}
}

void SceneManager::Update() {
	// ���݂̃V�[���̍X�V����
	myScene->Update();
}

void SceneManager::Draw() {
	// ���݂̃V�[���̕`�揈��
	myScene->Draw();
}