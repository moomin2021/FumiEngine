#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Object3D.h"
#include "Camera.h"
#include "Key.h"
#include "Sprite.h"
#include <memory>

class TemplateScene : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_;

<<<<<<< HEAD:TitleScene.h
	std::unique_ptr<Sprite> titleS_;
	int titleImage_;

	std::unique_ptr<Sprite> spaceStartText_;
	int spaceStartTextH_;

=======
>>>>>>> master:TemplateScene.h
	// �J����
	Camera* camera_;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	TemplateScene();

	// �f�X�g���N�^
	~TemplateScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
};

