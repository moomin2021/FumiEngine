#pragma once
// DirectXInput
#include <dinput.h>

// ComPtr�p
#include <wrl.h>
using namespace Microsoft::WRL;

// WinAPI
#include "WinAPI.h"

// Vector2
#include "Vector2.h"

enum MouseButton {
	M_LEFT,
	M_RIGHT,
	M_CENTER
};

class Mouse {
	// �����o�ϐ�
public:

private:
	// �f�o�C�X
	ComPtr<IDirectInputDevice8> device_;

	// ���݂̃}�E�X�̓��͏��
	DIMOUSESTATE nowMouse_;

	// �O�t���[���̃}�E�X�̓��͏��
	DIMOUSESTATE oldMouse_;

	// �}�E�X�J�[�\�����W
	POINT p_;

	// WinAPI
	WinAPI* win_;

	// �����o�֐�
public:
	// �C���X�^���X�ǂݍ���
	static Mouse* GetInstance();

	// ����������
	void Initialize(WinAPI* win);

	// �X�V����
	void Update();

	/// <summary>
	/// �w�肳�ꂽ�}�E�X�̃{�^����������Ă�����A1��Ԃ��A�����łȂ������ꍇ0��Ԃ�
	/// </summary>
	/// <param name="button"> ���肵�����}�E�X�{�^�� </param>
	/// <returns></returns>
	bool PushMouseButton(MouseButton button) { return nowMouse_.rgbButtons[button] == (0x80); }

	/// <summary>
	/// �w�肳�ꂽ�}�E�X�̃{�^���������ꂽ�u�Ԃ�������A1��Ԃ��A�����łȂ������ꍇ0��Ԃ�
	/// </summary>
	/// <param name="button"> ���肵�����}�E�X�{�^�� </param>
	/// <returns></returns>
	bool TriggerMouseButton(MouseButton button) { return nowMouse_.rgbButtons[button] == (0x80) && !(oldMouse_.rgbButtons[button] == (0x80)); }

	// --�}�E�X�̈ʒu-- //
	Vector2 MousePos() { return Vector2(p_.x, p_.y); }

	// --�}�E�X�̈ړ���-- //
	Vector2 GetMouseVelosity() { return Vector2((float)nowMouse_.lX, (float)nowMouse_.lY); }

private:
	// �R���X�g���N�^
	Mouse();

	// �f�X�g���N�^
	~Mouse();

};