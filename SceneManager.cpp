#include "SceneManager.h"
#include "Scene1.h"
#include "DX12Cmd.h"
#include "PipelineManager.h"

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
	nowScene_ = std::make_unique<Scene1>();
	nowScene_->Initialize();

	highLumiPostEffect_ = std::make_unique<PostEffect>();
	bloomPostEffect_ = std::make_unique<PostEffect>();

	//nowPostEffect_ = std::move(postEffect_);

	PipelineManager::GetInstance();
}

// �f�X�g���N�^
SceneManager::~SceneManager() {
	
}

void SceneManager::ChangeScene(int changeSceneNum)
{
	switch (changeSceneNum)
	{
	case SCENE::SCENE1:
		nowScene_ = std::make_unique<Scene1>();
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
	highLumiPostEffect_->PreDraw();

	nowScene_->Draw();

	highLumiPostEffect_->PostDraw();

	bloomPostEffect_->PreDraw();

	PipelineManager::GetInstance()->PreDraw("HighLumi");
	highLumiPostEffect_->Draw();

	bloomPostEffect_->PostDraw();

	// --�`��O����-- //
	DX12Cmd::GetInstance()->PreDraw();

	PipelineManager::GetInstance()->PreDraw("Bloom");

	bloomPostEffect_->Draw();

	// --�`��㏈��-- //
	DX12Cmd::GetInstance()->PostDraw();
}
