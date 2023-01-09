#include "WinAPI.h"
#pragma comment(lib, "winmm.lib")

// --�E�B���h�E�v���V�[�W���v���g�^�C�v�錾-- //
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// --�C���X�^���X�ǂݍ���-- //
WinAPI* WinAPI::GetInstance() {
	// --�C���X�^���X����-- //
	static WinAPI win;

	// --�C���X�^���X��Ԃ�-- //
	return &win;
}

// --�E�B���h�E�T�C�Y-- //
const int WinAPI::winWidth_ = 1280;
const int WinAPI::winHeight_ = 720;

// --�R���X�g���N�^-- //
WinAPI::WinAPI() :
#pragma region ���������X�g
	winClass_{},// -> �E�B���h�E�N���X
	winRect_{},// -> �E�B���h�E�T�C�Y
	hwnd_{},// -> �E�B���h�E�n���h��(�I�u�W�F�N�g)
	msg_{}// -> ���b�Z�[�W
#pragma endregion
{

}

// --�f�X�g���N�^-- //
WinAPI::~WinAPI() {
	
}

// --����������-- //
void WinAPI::Initialize() {
	// �V�X�e���^�C�}�[�̕���\���グ��
	timeBeginPeriod(1);

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
	AdjustWindowRect(&winRect_, WS_POPUP, false);

	// �f�B�X�v���C�̏����擾
	MONITORINFOEX monitorInfoEx;
	{
		HMONITOR hMonitor;
		POINT pt = { 100, 100 };

		hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);

		monitorInfoEx.cbSize = sizeof(monitorInfoEx);
		GetMonitorInfo(hMonitor, &monitorInfoEx);
	}

	// --�E�B���h�E�I�u�W�F�N�g�̐���-- //
	hwnd_ = CreateWindow(
		winClass_.lpszClassName,// ------> �N���X��
		L"DirectXGame",// -------> �^�C�g���o�[�̕���
		WS_POPUP,// --> �W���I�ȃE�B���h�E�X�^�C��
		monitorInfoEx.rcWork.left,
		monitorInfoEx.rcWork.top,
		monitorInfoEx.rcWork.right,
		monitorInfoEx.rcWork.bottom,
		nullptr,// --------------> �e�E�B���h�E�n���h��
		nullptr,// --------------> ���j���[�n���h��
		winClass_.hInstance,// ----------> �Ăяo���A�v���P�[�V�����n���h��
		nullptr);// -------------> �I�v�V����

	// --�E�B���h�E��\����Ԃɂ���-- //
	ShowWindow(hwnd_, SW_SHOW);

	// �}�E�X�J�[�\��������
	ShowCursor(false);

	// �}�E�X�|�C���^���E�B���h�E�̊O�ɍs���邩�ǂ����ݒ�
	ClipCursor(&monitorInfoEx.rcWork);
}

// --�I�����b�Z�[�W�����Ă��邩�Q��-- //
bool WinAPI::IsEndMessage() {
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
void WinAPI::WinClassUnregister() { UnregisterClass(winClass_.lpszClassName, winClass_.hInstance); }

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