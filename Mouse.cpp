#include "Mouse.h"
#include <cassert>
#define DIREXTINPUT_VERSION 0x0800// DirectInput�̃o�[�W�����w��
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Mouse* Mouse::GetInstance()
{
	// �֐���static�ϐ��Ƃ��Đ錾
	static Mouse instance;
	return &instance;
}

Mouse::Mouse() :
#pragma region ���������X�g
	device_(nullptr),// -> �f�o�C�X
	nowMouse_{},// -> ���݂̃}�E�X�̏��
	oldMouse_{}// -> �O�t���[���̃}�E�X�̏��
#pragma endregion
{

}

Mouse::~Mouse()
{
	// �f�o�C�X����
	device_->Unacquire();

	// �f�o�C�X�̉��
	device_->Release();
}

void Mouse::Initialize(WinAPI* win)
{
	win_ = win;

	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// DirectInput�f�o�C�X����
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(
		win->GetWNDCLASSEX().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr
	);
	assert(SUCCEEDED(result));

	// �}�E�X�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysMouse, &device_, NULL);
	assert(SUCCEEDED(result));

	// �}�E�X���̓f�[�^�̌`���̃Z�b�g
	result = device_->SetDataFormat(&c_dfDIMouse);// �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = device_->SetCooperativeLevel(
		win->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));

	// �}�E�X�f�o�C�X����J�n
	device_->Acquire();

	// DirectInput�f�o�C�X�̉��
	directInput->Release();
}

void Mouse::Update()
{
	// --�O�t���[���̃}�E�X�̓��͏�Ԃ�ۑ�-- //
	oldMouse_ = nowMouse_;

	// --�}�E�X�̓��͏�Ԃ��擾-- //
	device_->GetDeviceState(sizeof(DIMOUSESTATE), &nowMouse_);

	// --�}�E�X�̍��W���擾-- //
	GetCursorPos(&p_);
	ScreenToClient(FindWindowW(win_->GetWNDCLASSEX().lpszClassName, nullptr), &p_);
}
