#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "ClearScene.h"

BaseScene* SceneManager::nowScene_ = nullptr;

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
	nowScene_ = new TitleScene();
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
	case SCENE::TITLE:
		nowScene_ = new TitleScene();
		nowScene_->Initialize();
		break;
	case SCENE::GAME:
		nowScene_ = new GameScene();
		nowScene_->Initialize();
		break;
	case SCENE::CLEAR:
		nowScene_ = new ClearScene();
		nowScene_->Initialize();
		break;
	}
}

// �X�V����
void SceneManager::Update() {

	if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE::TITLE);
	if (key_->TriggerKey(DIK_2)) ChangeScene(SCENE::GAME);
	if (key_->TriggerKey(DIK_3)) ChangeScene(SCENE::CLEAR);

	nowScene_->Update();
}

// �`�揈��
void SceneManager::Draw()
{
	nowScene_->Draw();
}
