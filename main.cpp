#pragma region ���N���X�ǂݍ���(�܂߂�)
#include "FMWinAPI.h"// -> WinAPI
#pragma endregion


// --Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)-- //
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// --WinAPI�N���X-- //
	FMWinAPI* winAPI = FMWinAPI::GetInstance();// -> �C���X�^���X�擾
	winAPI->Initialize();// -> ����������

	// --�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

	// --�Q�[�����[�v-- //
	while (true) {
		// --�I�����b�Z�[�W�����Ă����烋�[�v�I��-- //
		if (winAPI->IsEndMessage() == true) break;


	}

	// --�E�B���h�E�N���X�o�^����-- //
	winAPI->WinClassUnregister();

#pragma region ���������
	winAPI->Release();// -> WinAPI
#pragma endregion

	return 0;
}