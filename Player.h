#pragma once
#include "Key.h"
#include "Mouse.h"

class Player
{
#pragma region �����o�ϐ�
private:
	// ���̓N���X�C���X�^���X
	Key* key_;		// �L�[�{�[�h
	Mouse* mouse_;	// �}�E�X
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �v���C���[
	/// </summary>
	Player();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();
#pragma endregion
};