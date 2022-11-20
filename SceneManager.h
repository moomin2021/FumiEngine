#pragma once
#include "BaseScene.h"

class SceneManager {
public:
	// �V�[���񋓌^
	enum SCENE {
		SCENE1,
	};

	// �V�[���������̃V�[���ɕς���
	static void ChangeScene(SCENE scene);

	// ���݂̃V�[���̍X�V����
	static void Update();

	// ���݂̃V�[���̕`�揈��
	static void Draw();

private:
	// ���݂̃V�[��
	static BaseScene* myScene;
};