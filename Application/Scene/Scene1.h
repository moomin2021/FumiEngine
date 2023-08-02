#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Sound.h"
#include "ParticleManager.h"
#include "Camera.h"

#include <memory>

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_ = nullptr;

	std::unique_ptr<Camera> camera_ = nullptr;

	// �p�[�e�B�N��
	std::unique_ptr<ParticleManager> particle_ = nullptr;

	// �p�[�e�B�N���摜
	uint16_t particleHandle_ = 0;

	// �T�E���h
	uint16_t soundKey_;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene1();

	// �f�X�g���N�^
	~Scene1();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
};

