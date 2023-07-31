#pragma once
#include "BaseScene.h"// -> �e�V�[���̃x�[�X
#include "Key.h"// -> �L�[�{�[�h����
#include "PostEffect.h"

#include <memory>

// �V�[���񋓌^
enum SCENE {
	TITLE,
	SCENE1,
	GAMEOVER,
	TRANSITION,
};

enum PostEffectType {
	NORMAL,
	BLUR,
	BLOOM
};

class SceneManager {
	// --�����o�ϐ�-- //
public:

private:
	// �L�[�{�[�h���̓N���X
	Key* key_;

	// ���݂̃V�[��
	std::unique_ptr<BaseScene> nowScene_;

	// �|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> gaussianPostEffect_;
	std::unique_ptr<PostEffect> highLumiPostEffect_;
	std::unique_ptr<PostEffect> bloomPostEffect_;

	SCENE nextScene_ = SCENE1;

	// �|�X�g�G�t�F�N�g�̎��
	PostEffectType postEffectType_;

	// �V�[����ύX����
	bool isChangeScene_ = false;

	// --�����o�֐�-- //
public:
	// �C���X�^���X�擾
	static SceneManager* GetInstance();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �V�[���؂�ւ�
	void ChangeScene(SCENE changeSceneNum);

	void SceneTransition(SCENE scene);

	SCENE GetNextScene() { return nextScene_; }

private:
	// �R���X�g���N�^
	SceneManager();

	// �f�X�g���N�^
	~SceneManager();

	// --�֎~-- //
public:
	// �R�s�[�R���X�g���N�^
	SceneManager(const SceneManager& instance) = delete;

	// ������Z�q
	SceneManager& operator=(const SceneManager& instance) = delete;
};