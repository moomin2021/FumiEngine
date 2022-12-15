#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"

// �C���X�^���X�擾
SceneManager* SceneManager::GetInstance()
{
	// �֐���static�ϐ��Ƃ��Đ錾
	static SceneManager instance;
	return &instance;
}

void SceneManager::ChangeScene(int sceneNo)
{
	// ���݂̃V�[������ł͂Ȃ�������
	//if (nowScene_ != nullptr) {
	//	// ���݂̃V�[�������������
	//	delete nowScene_;
	//}
	delete nowScene_;

	// �w�肳�ꂽ�V�[���̃C���X�^���X�����Ə�����
	switch (sceneNo)
	{
	case SCENE::TITLE:
		nowScene_ = new TitleScene();
		break;
	case SCENE::GAME:
		nowScene_ = new GameScene();// -> �C���X�^���X����
		break;
	}
	nowScene_->Initialize();
}

// �R���X�g���N�^
SceneManager::SceneManager() {
	// �C���X�^���X�擾
	key_ = Key::GetInstance();

	// �ŏ��̃V�[��
	nowScene_ = new GameScene();
	nowScene_->Initialize();
}

// �f�X�g���N�^
SceneManager::~SceneManager() {
	delete nowScene_;
}

// �X�V����
void SceneManager::Update() {
	if (key_->TriggerKey(DIK_1)) {
		ChangeScene(SCENE::TITLE);
	}
	if (key_->TriggerKey(DIK_2)) ChangeScene(SCENE::GAME);

	nowScene_->Update();
}

// �`�揈��
void SceneManager::Draw()
{
	nowScene_->Draw();
}
