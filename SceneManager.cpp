#include "SceneManager.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"

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
	case SCENE::SCENE2:
		nowScene_ = new Scene2();
		nowScene_->Initialize();
		break;
	case SCENE::SCENE3:
		nowScene_ = new Scene3();
		nowScene_->Initialize();
		break;
	case SCENE::SCENE4:
		nowScene_ = new Scene4();
		nowScene_->Initialize();
		break;
	}
}

// �X�V����
void SceneManager::Update() {

	if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE::SCENE1);
	if (key_->TriggerKey(DIK_2)) ChangeScene(SCENE::SCENE2);
	if (key_->TriggerKey(DIK_3)) ChangeScene(SCENE::SCENE3);
	if (key_->TriggerKey(DIK_4)) ChangeScene(SCENE::SCENE4);

	nowScene_->Update();
}

// �`�揈��
void SceneManager::Draw()
{
	nowScene_->Draw();
}
