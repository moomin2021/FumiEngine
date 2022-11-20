#include "Key.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Key* Key::GetInstance() {
	// �C���X�^���X����
	static Key key;

	// --�C���X�^���X��Ԃ�-- //
	return &key;
}

Key::~Key() {
	// �L�[�{�[�h�f�o�C�X�̐��䐧��
	device_->Unacquire();

	// �L�[�{�[�h�f�o�C�X�̉��
	device_->Release();
}

void Key::Initialize(WinAPI * win) {
	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

#pragma region DirectInput�̏�����

	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(
		win->GetWNDCLASSEX().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr
	);
	assert(SUCCEEDED(result));

#pragma endregion
	
#pragma region �f�o�C�X�̐���

	// �f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &device_, NULL);
	assert(SUCCEEDED(result));

	// �L�[�{�[�h���̓f�[�^�̌`���̃Z�b�g
	result = device_->SetDataFormat(&c_dfDIKeyboard);// �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = device_->SetCooperativeLevel(
		win->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));

	// �f�o�C�X����J�n
	device_->Acquire();

#pragma endregion
}

void Key::Update() {
	// �O�t���[���̃L�[�̏�Ԃ�ۑ�
	for (size_t i = 0; i < 256; i++) oldKeys_[i] = keys_[i];

	// �S�L�[�̓��͏�Ԃ��擾����
	device_->GetDeviceState(sizeof(keys_), keys_);
}