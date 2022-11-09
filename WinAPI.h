#pragma once
// --WindowsAPI-- //
#include <Windows.h>

class WinAPI {
#pragma region �����o�ϐ�
public:

private:
	// --�E�B���h�E�T�C�Y-- //
	static const int winWidth_;// --> ����
	static const int winHeight_;// -> �c��

	WNDCLASSEX winClass_;// -> �E�B���h�E�N���X
	RECT winRect_;// -> �E�B���h�E�T�C�Y
	HWND hwnd_;// -> �E�B���h�E�n���h��(�I�u�W�F�N�g)
	MSG msg_;// -> ���b�Z�[�W

#pragma endregion

#pragma region �����o�֐�
public:
	// --�C���X�^���X�ǂݍ���-- //
	static WinAPI* GetInstance();

	// --����������-- //
	void Initialize();

	// --�I�����b�Z�[�W�����Ă��邩�Q��-- //
	bool IsEndMessage();

	// --�E�B���h�E�N���X�o�^����-- //
	void WinClassUnregister();

#pragma region �Q�b�^�[
	// --�E�B���h�D�N���X���擾-- //
	WNDCLASSEX GetWNDCLASSEX() { return winClass_; }

	// --�E�B���h�E�n���h��(�I�u�W�F�N�g)���擾-- //
	HWND GetHWND() { return hwnd_; }

	// --�E�B���h�E�̉��T�C�Y���擾-- //
	static int GetWidth() { return winWidth_; }

	// --�E�B���h�E�̏c�T�C�Y���擾-- //
	static int GetHeight() { return winHeight_; }
#pragma endregion

private:
	// --�R���X�g���N�^-- //
	WinAPI();

	// --�f�X�g���N�^-- //
	~WinAPI();
#pragma endregion

	// --�֎~-- //
	WinAPI(const WinAPI&) = delete;// --------------> �R�s�[�R���X�g���N�^�֎~
	WinAPI& operator = (const WinAPI&) = delete;// -> �R�s�[������Z�q�֎~
};