#include "SceneManager.h"
<<<<<<< HEAD
#include "GameScene.h"
#include "TitleScene.h"

BaseScene* SceneManager::nowScene_ = nullptr;
=======
#include "Scene1.h"
>>>>>>> master

// �C���X�^���X�擾
SceneManager* SceneManager::GetInstance()
{
	// �֐���static�ϐ��Ƃ��Đ錾
	static SceneManager instance;
	return &instance;
}

// �R���X�g���N�^
SceneManager::SceneManager() {
	// �C���X�^���X�擾
	key_ = Key::GetInstance();

	// �ŏ��̃V�[��
	nowScene_ = new Scene1();
	nowScene_->Initialize();
}

// �f�X�g���N�^
SceneManager::~SceneManager() {
	delete nowScene_;
}

void SceneManager::ChangeScene(int changeSceneNum)
{
	// ���݂̃V�[�����������
	if (nowScene_ != nullptr) {
		delete nowScene_;
	}

	switch (changeSceneNum)
	{
	case SCENE::SCENE1:
		nowScene_ = new Scene1();
		nowScene_->Initialize();
		break;
	}
}

// �X�V����
void SceneManager::Update() {

<<<<<<< HEAD
	if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE::TITLE);
	if (key_->TriggerKey(DIK_2)) ChangeScene(SCENE::GAME);
<<<<<<< HEAD
	if (key_->TriggerKey(DIK_3)) ChangeScene(SCENE::CLEAR);
=======
	if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE::SCENE1);
>>>>>>> master
=======
>>>>>>> parent of fcb6802 (終了)

	nowScene_->Update();
}

// �`�揈��
void SceneManager::Draw()
{
	nowScene_->Draw();
}
