#include "SceneManager.h"
#include "Scene1.h"

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

	if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE::SCENE1);

	nowScene_->Update();
}

// �`�揈��
void SceneManager::Draw()
{
	nowScene_->Draw();
}
