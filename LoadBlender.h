#pragma once
#include <string>
#include <map>
#include <vector>

class LoadBlender
{
#pragma region �\����

#pragma endregion

#pragma region �����o�ϐ�

#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	LoadBlender();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// Json�t�@�C���ǂݍ���
	/// </summary>
	void LoadJson(std::string fileName);
#pragma endregion
};