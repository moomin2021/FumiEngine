#pragma once
class DX12Cmd
{
#pragma region �����o�ϐ�
public:

private:
	// --�C���X�^���X-- //
	static DX12Cmd* myInstance_;
#pragma endregion

#pragma region �����o�֐�
public:
	// --�C���X�^���X�ǂݍ���-- //
	static DX12Cmd* GetInstance();

	// --�C���X�^���X���-- //
	void Release();

	// --����������-- //
	void Initialize();

#pragma region �Q�b�^�[
	
#pragma endregion

private:
	// --�R���X�g���N�^-- //
	DX12Cmd();

	// --�f�X�g���N�^-- //
	~DX12Cmd();
#pragma endregion

	// --�֎~-- //
	DX12Cmd(const DX12Cmd&) = delete;// --------------> �R�s�[�R���X�g���N�^�֎~
	DX12Cmd& operator = (const DX12Cmd&) = delete;// -> �R�s�[������Z�q�֎~
};

