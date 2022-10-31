#include "FMWinAPI.h"

// --�E�B���h�E�v���V�[�W���v���g�^�C�v�錾-- //
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// --�C���X�^���X�̏�����-- //
FMWinAPI* FMWinAPI::myInstance_ = nullptr;

// --�C���X�^���X�ǂݍ���-- //
FMWinAPI* FMWinAPI::GetInstance() {
	// --�C���X�^���X�����������琶������-- //
	if (myInstance_ == nullptr) myInstance_ = new FMWinAPI();

	// --�C���X�^���X��Ԃ�-- //
	return myInstance_;
}

// --�C���X�^���X���-- //
void FMWinAPI::Release() {
	// --�C���X�^���X�����������牽�������ɏI������-- //
	if (myInstance_ == nullptr) return;

	// --�C���X�^���X���-- //
	delete myInstance_;
	myInstance_ = nullptr;
}

// --�R���X�g���N�^-- //
FMWinAPI::FMWinAPI() :
#pragma region �萔������
	// --�E�B���h�E�T�C�Y-- //
	winWidth_(1280),// -> ����
	winHeight_(720)// --> �c��
#pragma endregion
{

}

// --�f�X�g���N�^-- //
FMWinAPI::~FMWinAPI() {
	
}

// --����������-- //
void FMWinAPI::Initialize() {
	// --�E�B���h�E�N���X�̐ݒ�-- //
	winClass_.cbSize = sizeof(WNDCLASSEX);// -> WNDCLASSEX�\���̂̃T�C�Y
	winClass_.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�(�A�h���X����)
	winClass_.lpszClassName = L"DirectXGame"; // �E�B���h�E�N���X��
	winClass_.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	winClass_.hCursor = LoadCursor(NULL, IDC_ARROW); // �}�E�X�J�[�\���w��

	// --�E�B���h�E�N���X��OS�ɓo�^����-- //
	RegisterClassEx(&winClass_);

	// --�E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }-- //
	winRect_ = { 0, 0, winWidth_, winHeight_ };

	// --�����ŃT�C�Y��␳����-- //
	AdjustWindowRect(&winRect_, WS_OVERLAPPEDWINDOW, false);

	// --�E�B���h�E�I�u�W�F�N�g�̐���-- //
	hwnd_ = CreateWindow(
		winClass_.lpszClassName,// ------> �N���X��
		L"DirectXGame",// -------> �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,// --> �W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,// --------> �\��X���W(OS�ɔC����)
		CW_USEDEFAULT,// --------> �\��Y���W(OS�ɔC����)
		winRect_.right - winRect_.left,// -> �E�B���h�E����
		winRect_.bottom - winRect_.top,// -> �E�B���h�E�c��
		nullptr,// --------------> �e�E�B���h�E�n���h��
		nullptr,// --------------> ���j���[�n���h��
		winClass_.hInstance,// ----------> �Ăяo���A�v���P�[�V�����n���h��
		nullptr);// -------------> �I�v�V����

	// --�E�B���h�E��\����Ԃɂ���-- //
	ShowWindow(hwnd_, SW_SHOW);
}

// --�I�����b�Z�[�W�����Ă��邩�Q��-- //
bool FMWinAPI::IsEndMessage() {
	// --���b�Z�[�W�����邩-- //
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg_); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	// --�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���-- //
	if (msg_.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

// --�E�B���h�E�N���X�o�^����-- //
void FMWinAPI::WinClassUnregister() { UnregisterClass(winClass_.lpszClassName, winClass_.hInstance); }

// --�E�B���h�E�v���V�[�W��(�V�X�e�����b�Z�[�W���������邽�߂̊֐�)-- //
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
		// �E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		// OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	// �W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}