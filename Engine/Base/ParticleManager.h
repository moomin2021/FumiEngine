#pragma once
#include "Matrix4.h"
#include "float4.h"

#include <wrl.h>

class ParticleManager
{
#pragma region �����o�ϐ�
private:

#pragma endregion

#pragma region �����o�֐�
public:
	// �C���X�^���X���擾
	ParticleManager* GetInstance();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �R���X�g���N�^
	ParticleManager();
#pragma endregion
};