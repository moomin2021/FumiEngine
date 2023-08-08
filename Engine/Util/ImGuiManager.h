#pragma once
class ImGuiManager
{
#pragma region �����o�ϐ�
private:
#pragma endregion

#pragma region �����o�֐�
public:
	// �C���X�^���X�擾
	static ImGuiManager* GetInstance();

	// ����������
	void Initialize();

	// ImGui��t�J�n
	void Begin();

	// ImGui��t�I��
	void End();

	// ��ʂւ̕`��
	void Draw();

private:
	// �R���X�g���N�^
	ImGuiManager();

	// �f�X�g���N�^
	~ImGuiManager();
#pragma endregion

#pragma region ����֐�
	// �֎~
	ImGuiManager(const ImGuiManager&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	ImGuiManager& operator = (const ImGuiManager&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};