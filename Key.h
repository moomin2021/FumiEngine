#pragma once
// --DirectXInput-- //
#define DIREXTINPUT_VERSION 0x0800// DirectInput�̃o�[�W�����w��
#include <dinput.h>

// --ComPtr�p-- //
#include <wrl.h>

#include "WinAPI.h"

class Key {
	// namespace�̏ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// --�����o�֐�-- //
public:
	// �C���X�^���X�ǂݍ���
	static Key* GetInstance();

	// ����������
	void Initialize(WinAPI * win);

	// �X�V����
	void Update();

	/// <summary>
	/// �w�肳�ꂽ�L�[��������Ă�����A1��Ԃ��A�����łȂ�������0��Ԃ�
	/// </summary>
	/// <param name="key"> ���肵�����L�[ </param>
	/// <returns></returns>
	bool PushKey(UINT key) { return keys_[key]; }

	/// <summary>
	/// �w�肳�ꂽ�L�[�������ꂽ�u�Ԃ�������A1��Ԃ��A�����łȂ�������0��Ԃ�
	/// </summary>
	/// <param name="key"> ���肵�����L�[ </param>
	/// <returns></returns>
	bool TriggerKey(UINT8 key) { return keys_[key] && !oldKeys_[key]; }

private:
	// �f�X�g���N�^
	~Key();

	// --�����o�ϐ�-- //
public:
	
private:
	// ���݂̃L�[�{�[�h�̏��
	UINT8 keys_[256];

	// �O�t���[���̃L�[�{�[�h�̏��
	UINT8 oldKeys_[256];

	// �f�o�C�X
	ComPtr<IDirectInputDevice8> device_;

};