#pragma once
#include "BaseScene.h"
#include "Key.h"
#include <memory>

class SceneManager {
	// --�����o�֐�-- //
public:
	// �V�[���񋓌^
	enum SCENE {
		SCENE1,
	};

	// �R���X�g���N�^
	SceneManager();

	// �f�X�g���N�^
	~SceneManager();

	// �V�[���������̃V�[���ɕς���
	static void ChangeScene(SCENE scene);

	// ���݂̃V�[���̍X�V����
	static void Update();

	// ���݂̃V�[���̕`�揈��
	static void Draw();

private:

	// --�����o�ϐ�-- //
public:

private:
	// ���݂̃V�[��
	static BaseScene* myScene_;

	// �L�[�{�[�h����
	Key* key_;
};