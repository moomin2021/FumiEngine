#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "Boss.h"

#include <memory>

class BossGenerator
{
#pragma region �����o�ϐ�
private:
	// �W�F�l���[�^�[
	std::unique_ptr<Model> mGen_;// ���f��
	std::unique_ptr<Object3D> oGen_;// �I�u�W�F�N�g
	std::unique_ptr<SphereCollider> cGen_;// �R���C�_�[

	// �{�X
	std::unique_ptr<Boss> boss_;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	BossGenerator();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
#pragma endregion
};